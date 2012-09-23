#ifndef MA_COMMANDFLAGS_HH
#define MA_COMMANDFLAGS_HH

#include <maya/MSyntax.h>
#include <maya/MArgList.h>

#include <string>
#include <list>

namespace ma {

class CommandFlags
{
public:

    CommandFlags( const char* name, const char* description );

    operator MSyntax();

    bool handle( const MArgList& args );

    void addFlag(
            const char* shortName,
            const char* longName,
            const char* category,
            const char* description
            );

    MStatus setObjectType( MSyntax::MObjectFormat objectFormat, unsigned int min, unsigned int max )
    {
        return m_syntax.setObjectType( objectFormat, min, max );
    }

private:

    struct Flag
    {
        Flag( const char* s, const char* l, const char* c, const char* d )
         : shortName( s ), longName( l ), category( c ), description( d ) {}

        std::string shortName;
        std::string longName;
        std::string category;
        std::string description;
    };

private:

    std::string m_name;
    std::string m_description;

    MSyntax m_syntax;
    MSyntax m_noArgsSyntax;

    std::list< Flag > m_flags;

};

}

#endif // MA_COMMANDFLAGS_HH

