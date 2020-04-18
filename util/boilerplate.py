
import os
import ntpath
import re

# include path
include_path = os.path.abspath(os.path.join(os.path.dirname(os.path.realpath(__file__)), '../include'))

# source path
src_path = os.path.abspath(os.path.join(os.path.dirname(os.path.realpath(__file__)), '../src'))

# hydro core path
hydro_path = os.path.abspath(os.path.join(os.path.dirname(os.path.realpath(__file__)), '../hydro'))

# unit test path
tests_path = os.path.abspath(os.path.join(os.path.dirname(os.path.realpath(__file__)), '../tests'))


# search paths
paths_to_search = [
    include_path, 
    src_path,
    hydro_path,
    tests_path
]

# template string will be stored here
template = ''

# number of files processed
files_processed = 0

def load_boilerplate(path):
    file = open(path, 'r')
    if file:
        return file.read().strip()
    else:
        return '' #fail

def load_cpp_files(path, results):
    allfiles = os.listdir(path)
    for file in allfiles:
        filepath = os.path.join(path, file)
        if os.path.isdir(filepath):
            # load files recursively
            load_cpp_files(filepath, results)
        elif file.startswith('.') == False and (file.endswith('.h') or file.endswith('.hpp') or file.endswith('.c') or file.endswith('.cpp') or file.endswith('.hydro')):
            # push result
            results.append(filepath)

def find_cpp_files(path, results):
    load_cpp_files(path, results)
    return results

def apply_template(boilerplate, fullpath, basename):
    return boilerplate.replace("__file__", fullpath).replace("__filename__", basename)

def apply_boilerplate(boilerplate, code):
    i = 0
    n = len(code)
    result = ''
    comment = True
    while i < n:
        ch = code[i]
        i = i + 1 # to next char
        if comment:
            nxt = code[i] if i < n else ''
            if ch == '/' and nxt == '/':
                while code[i] != "\n" and i < n:
                    i = i + 1 # skip single line comment
            elif ch == '/' and nxt == '*':
                i = i + 1 # skip "*"
                while i + 1 < n:
                    if code[i] == '*':
                        if code[i + 1] == '/':
                            i = i + 2
                            break # end
                    i = i + 1
            elif re.search("^[\\s]$", ch):
                pass # skip whitespace
            else:
                comment = False
                result = boilerplate + "\n\n" + ch # prepend boilerplate
        else:
            result += ch
    return result

def apply_to_file(cpp):
    global template
    global files_processed
    file = open(cpp, 'r')
    code = file.read()
    basename = ntpath.basename(cpp)
    boilerplate = apply_template(template, cpp, basename)
    if code.startswith(boilerplate):
        return False
    else:
        code = apply_boilerplate(boilerplate, code)
        files_processed = files_processed + 1
        file.close()
        file = open(cpp, 'w') # write
        file.write(code)
        file.close()
        return True

def traverse_files(paths_to_search):
    global files_processed
    all_cpp_files = []
    for search_path in paths_to_search:
        for cpp in find_cpp_files(search_path, all_cpp_files):
            file = open(cpp, 'r')
            if apply_to_file(cpp):    
                print(cpp)

txtfile = os.path.abspath(os.path.join(os.path.dirname(os.path.realpath(__file__)), 'boilerplate.txt'))
template = load_boilerplate(txtfile)

# start program
if __name__ == '__main__':
    print("program commencing...")
    traverse_files(paths_to_search)
    print(str(files_processed) + " file(s) processed")
    print("program complete!")
# end program