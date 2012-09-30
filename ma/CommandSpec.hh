#ifndef MA_COMMANDSPEC_HH
#define MA_COMMANDSPEC_HH

#include <maya/MSyntax.h>
#include <maya/MArgList.h>

#include <string>
#include <vector>

namespace ma {

namespace commandproperties {

enum Property
{
    kNone      = 0x00,
    kCreate    = 0x01,
    kQuery     = 0x02,
    kEdit      = 0x04,
    kMultiUse  = 0x08
};

typedef uint32_t PropertyBitMask;

}

class CommandSpec
{
public:

    CommandSpec( const char* name, const char* arguments, const char* description );

    MSyntax syntax();

    bool handle( const MArgList& args );

    void addFlag(
            const std::string& shortName,
            const std::string& longName,
            const std::string& category,
            commandproperties::PropertyBitMask properties,
            const std::string& description,
            MSyntax::MArgType argType1=MSyntax::kNoArg,
            MSyntax::MArgType argType2=MSyntax::kNoArg,
            MSyntax::MArgType argType3=MSyntax::kNoArg,
            MSyntax::MArgType argType4=MSyntax::kNoArg,
            MSyntax::MArgType argType5=MSyntax::kNoArg,
            MSyntax::MArgType argType6=MSyntax::kNoArg
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
           const std::string& d,
           const std::vector< std::string >& a,
           const std::vector< std::string >& p
           )
         : shortName( s ),
           longName( l ),
           category( c ),
           description( d ),
           arguments( a ),
           properties( p ) {}

        std::string shortName;
        std::string longName;
        std::string category;
        std::string description;
        std::vector< std::string > arguments;
        std::vector< std::string > properties;
    };

private:

    std::string m_name;
    std::string m_arguments;
    std::string m_description;

    MSyntax m_syntax;
    MSyntax m_noArgsSyntax;

    std::vector< Flag > m_flags;
    std::vector< std::string > m_categories;
};

}

#endif // MA_COMMANDSPEC_HH

