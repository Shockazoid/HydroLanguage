
import os
import boilerplate

# variable
lib_path = os.path.abspath(os.path.join(os.path.dirname(os.path.realpath(__file__)), '../hydro'))

# target file
target = os.path.abspath(os.path.join(os.path.dirname(os.path.realpath(__file__)), '../include/hydro/hydro_native.hpp'))

# data
ids = []
qnames = []
paths = []
scripts = []

# converts a string to a cpp encoded string
def escape_cpp_string(value):
    newString = ''
    for ch in value:
        if ch == '"': # double quotes
            newString += "\\\""
        elif ch == "\n": # linefeed
            newString += "\\n"
        elif ch == "\\":
            newString += "\\\\"
        else: # everything else
            newString += ch
    return newString

# determines the relative path of a directory based on the target path
def compute_relative_path(targetPath, dirPath):
    if len(targetPath) > len(dirPath):
        return '' # invalid
    relativePath =  dirPath[len(targetPath):len(dirPath)]
    if not relativePath:
        return '/'
    return relativePath

# convert relative path to a Hydro Assembly qualified name (system/Object)
def path_to_qname(relativePath):
    qname = relativePath[0:len(relativePath) - len('.hydro')]
    return qname[1:len(qname)] # omit leading /

def qname_to_id(qn):
    sid = ''
    for ch in qn:
        if ch == '/':
            sid += '_'
        else:
            sid += ch
    return sid

def load_source_files(path):
    allfiles = os.listdir(path)
    result = []
    for file in allfiles:
        fullpath = os.path.join(path, file)
        if file.endswith('.hydro') and file[0] != '.':
            result.append(fullpath)
    return result

def load_sub_directories(path):
    allfiles = os.listdir(path)
    result = []
    for file in allfiles:
        fullpath = os.path.join(path, file)
        if os.path.isdir(fullpath) and file[0] != '.':
            result.append(fullpath)
    return result

def id_exists(sid):
    return sid in ids

# traverse directory
def traverse_directory(targetPath, path):
    hydro_files = load_source_files(path)
    sub_folders = load_sub_directories(path)
    # iterate hydro files
    for file in hydro_files:
        relative_path = compute_relative_path(targetPath, file)
        qn = path_to_qname(relative_path)
        sid = qname_to_id(qn)
        if id_exists(sid):
            continue
        with open(file, 'r') as file:
            content = file.read()
            ids.append(sid)
            qnames.append('"' + qn + '"')
            paths.append('"' + relative_path + '"')
            scripts.append('"' + escape_cpp_string(content) + '"')
    # iterate sub-folders
    for folder in sub_folders:
        traverse_directory(targetPath, os.path.join(path, folder))

def load_library_target(libTargetPath):
    abspath = os.path.abspath(libTargetPath)
    traverse_directory(abspath, abspath)

def generate_cpp():
    size = len(scripts)

    # namespace
    cpp = "namespace hydro\n"
    cpp += "{\n\n"
    
    # directives
    cpp += "#ifndef HYDRO_NATIVE_LIB\n"
    cpp += "#define HYDRO_NATIVE_LIB\n\n"

    # size
    cpp += "const int hydro_script_size = " + str(size) + ";\n\n"
    
    # raw scripts
    cpp += "const char **hydro_script_data = new const char*[" + str(size) + "]{" + ', '.join(scripts) + "};\n\n"
    
    # file paths
    cpp += "const char **hydro_script_path = new const char*[" + str(size) + "]{" + ', '.join(paths) + "};\n\n"
    cpp += "const char **hydro_script_qname = new const char*[" + str(size) + "]{" + ', '.join(qnames) + "};\n\n"
    
    # end direct and namespace
    cpp += "#endif /* HYDRO_NATIVE_LIB */\n\n"
    cpp += "} // namespace hydro\n\n"

    # finish
    return cpp

def save_result(result):
    global target
    if os.path.isfile(target):
        file = open(target, 'w') # override
    else:
        file = open(target, 'w+') # create file
    file.write(result)
    file.close()

load_library_target(lib_path)
result = generate_cpp()
save_result(result)
boilerplate.apply_to_file(target)
print('file generated successfully')
