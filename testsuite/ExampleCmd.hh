
#include <ma/CommandFlags.hh>

#include <maya/MPxCommand.h>


class ExampleCmd : public MPxCommand
{
public:

    static void* creator();
    static MSyntax syntaxCreator();

public:

    MStatus doIt( const MArgList& args );
    bool isUndoable() { return false; }
    bool hasSyntax() { return true; }

private:

    static ma::CommandFlags s_commandFlags;

};


