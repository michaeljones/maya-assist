
#include "ExampleCmd.hh"

#include <ma/PluginHandler.hh>

#include <maya/MFnPlugin.h>


MStatus handlePlugin( ma::PluginHandler& handler )
{
    MStatus status;

    status = handler.handleCommand(
            "exampleCommand",
            ExampleCmd::creator,
            ExampleCmd::syntaxCreator
            );

    return status;
}


MStatus initializePlugin( MObject obj )
{
    MStatus result;
    MFnPlugin plugin( obj, "Michael Jones", "0.1", "Any");

    ma::InitializePluginHandler handler( plugin );
    handlePlugin( handler );

    return result;
}

MStatus uninitializePlugin( MObject obj)
{
    MStatus result;

    MFnPlugin plugin( obj );

    ma::UninitializePluginHandler handler( plugin );
    handlePlugin( handler );

    return result;
}


