//
//                 __  __            __           
//                / / / /__  __ ____/ /_____ ____ 
//               / /_/ // / / // __  // ___// __ \
//              / __  // /_/ // /_/ // /   / /_/ /
//             /_/ /_/ \__, / \__,_//_/    \____/ 
//                    /____/                      
//
//              The Hydro Programming Language
//

#ifndef __h3o_cli__
#define __h3o_cli__

namespace hydro
{

enum h3o_cli_cmd
{
    h3o_invalid_cmd,
    h3o_default_cmd,
    h3o_exec_prog_cmd,
    h3o_build_cmd,
    h3o_project_cmd,
    h3o_version_cmd,
    h3o_help_cmd
};

class CliArgs
{
public:
    CliArgs()
    {
        m_nargs = 0;
        m_pos = -1;
        m_current = "";
    }
    CliArgs(int argc, const char *argv[])
    {
        m_nargs = argc;
        m_pos = -1;
        m_current = "";
        for(int i = 0; i < argc; i++)
            m_data.push_back(argv[i]);
    }
    CliArgs(const CliArgs &rhs)
    {
        m_nargs = rhs.m_nargs;
        m_pos = rhs.m_pos;
        m_current = rhs.m_current;
        m_data = rhs.m_data;
    }
    std::string read()
    {
        if(m_pos + 1 < m_nargs)
            return (m_current = m_data[++m_pos]);
        return "";
    }
    std::string current() const { return m_current; }
    bool willRead() const { return m_pos + 1 < m_nargs; }
    CliArgs &operator=(const CliArgs &rhs)
    {
        m_nargs = rhs.m_nargs;
        m_pos = rhs.m_pos;
        m_current = rhs.m_current;
        m_data = rhs.m_data;
        return (*this);
    }
private:
    int m_pos;
    int m_nargs;
    std::string m_current;
    std::vector<std::string> m_data;
};

class CliOptions
{
public:
    bool errorReporting{false};
    std::string path;
    std::string dir;
    bool abs{false}; // absolute path ?
    CliOptions() = delete;
    CliOptions(int argc, const char *argv[])
    {
        m_command = h3o_invalid_cmd;
        m_args = CliArgs{argc, argv};
        dir = m_args.read(); // read arg[0]
        parse();
    }
    ~CliOptions() {}
    bool invalid() const { return m_command == h3o_invalid_cmd; }
    std::string error() const { return m_error; }
    h3o_cli_cmd command() const { return m_command; }
    void parse()
    {
        std::string arg;
        
        if(willRead())
        {
            read(arg); // read next argument
            
            if(arg == "library")
            {
                
            }
            else if(arg == "build")
            {
                
            }
            else if(arg == "project")
            {
                if(willRead())
                {
                    read(arg); // get unexpected argument
                    m_error = "project command does not expect additional arguments '" + arg + "'.";
                    return; // abort
                }
                
                m_command = h3o_project_cmd;
            }
            else if(arg == "--version")
            {
                // parse version
                if(willRead())
                {
                    read(arg); // get unexpected argument
                    
                    // invalid argument
                    m_error = "--version does not expect additional arguments '" + arg + "'.";
                    return; // abort
                }
                
                m_command = h3o_version_cmd;
            }
            else if (arg == "--help")
            {
                // parse help
                if(willRead())
                {
                    read(arg); // get unexpected argument
                    m_error = "--help does not expect additional arguments '" + arg + "'.";
                    return; // abort
                }
                
                m_command = h3o_help_cmd;
            }
            else
            {
                while (!arg.empty())
                {
                    if (arg == "-q")
                    {
                        errorReporting = false;
                    }
                    else if(arg == "-a")
                    {
                        abs = true;
                    }
                    else if(path.empty())
                    {
                        path = arg;
                    }
                    else
                    {
                        m_error = "encountered unexpected additional argument '" + arg + "'";
                    }
                    
                    read(arg);
                }
                
                if(m_error.empty())
                    m_command = h3o_exec_prog_cmd;
            }
        }
        else
        {
            // default command
            m_command = h3o_default_cmd;
        }
    }
    
private:
    bool willRead() const { return m_args.willRead(); }
    void read(std::string &arg) { arg = m_args.read(); }
    CliArgs m_args;
    std::string m_error;
    h3o_cli_cmd m_command;
};

} // namespace hydro

#endif /* __h3o_cli__ */
