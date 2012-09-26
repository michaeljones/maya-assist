#ifndef MA_COMMANDFLAGS_HH
#define MA_COMMANDFLAGS_HH

#include <maya/MSyntax.h>
#include <maya/MArgList.h>

#include <string>
#include <vector>

namespace ma {

class CommandFlags
{
public:

    CommandFlags( const char* name, const char* arguments, const char* description );

    operator MSyntax();

    bool handle( const MArgList& args );

    void addFlag(
            const std::string& shortName,
            const std::string& longName,
            const std::string& category,
            const std::string& description
            );

    MStatus setObjectType( MSyntax::MObjectFormat objectFormat, unsigned int min, unsigned int max )
    {
        return m_syntax.setObjectType( objectFormat, min, max );
    }

private:

    struct Flag
    {
        Flag(
           const std::string& s,
           const std::string& l,
           const std::string& c,
           const std::string& d
           )
         : shortName( s ), longName( l ), category( c ), description( d ) {}

        std::string shortName;
        std::string longName;
        std::string category;
        std::string description;
    };

private:

    std::string m_name;
    std::string m_arguments;
    std::string m_description;

    MSyntax m_syntax;
    MSyntax m_noArgsSyntax;

    std::vector< Flag > m_flags;

};

}

#endif // MA_COMMANDFLAGS_HH

