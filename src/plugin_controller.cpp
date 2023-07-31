#include "plugin_controller.h"
#include "plugin_cids.h"
#include "vstgui/plugin-bindings/vst3editor.h"
#include "base/source/fstreamer.h"
#include "pluginterfaces/base/ibstream.h"


Steinberg::tresult PLUGIN_API PluginController::initialize(FUnknown* context)
{
	// Here the Plug-in will be instantiated
	//---do not forget to call parent ------
	Steinberg::tresult result = EditControllerEx1::initialize(context);
	if (result != Steinberg::kResultOk)
	{
		return result;
	}
	// Here you could register some parameters
    parameters.addParameter(STR16("Bypass"), nullptr, 1, 0,
                            Steinberg::Vst::ParameterInfo::kCanAutomate | Steinberg::Vst::ParameterInfo::kIsBypass,
                            PluginParams::kBypassId);

    return result;
}

Steinberg::tresult PLUGIN_API PluginController::terminate()
{
	// Here the Plug-in will be de-instantiated, last possibility to remove some memory!
	//---do not forget to call parent ------
	return EditControllerEx1::terminate ();
}

Steinberg::tresult PLUGIN_API PluginController::setComponentState(Steinberg::IBStream* state)
{
	// Here you get the state of the component (Processor part)
	if (!state)
		return Steinberg::kResultFalse;

    Steinberg::IBStreamer streamer(state, kLittleEndian);

    Steinberg::int32 bypassState;
    if (!streamer.readInt32(bypassState))
        return Steinberg::kResultFalse;
    setParamNormalized(kBypassId, bypassState ? 1 : 0);
	return Steinberg::kResultOk;
}

Steinberg::tresult PLUGIN_API PluginController::setState(Steinberg::IBStream* state)
{
	// Here you get the state of the controller
	return Steinberg::kResultTrue;
}

Steinberg::tresult PLUGIN_API PluginController::getState(Steinberg::IBStream* state)
{
	// Here you are asked to deliver the state of the controller (if needed)
	// Note: the real state of your plug-in is saved in the processor
	return Steinberg::kResultTrue;
}

Steinberg::IPlugView* PLUGIN_API PluginController::createView(Steinberg::FIDString name)
{
	// Here the Host wants to open your editor (if you have one)
	if (Steinberg::FIDStringsEqual (name, Steinberg::Vst::ViewType::kEditor))
	{
		// create your editor here and return a IPlugView ptr of it
		auto* view = new VSTGUI::VST3Editor (this, "view", "rsrc/plugin_editor.uidesc");
		return view;
	}
	return nullptr;
}

Steinberg::tresult PLUGIN_API PluginController::setParamNormalized (Steinberg::Vst::ParamID tag, Steinberg::Vst::ParamValue value)
{
	// called by host to update your parameters
	Steinberg::tresult result = EditControllerEx1::setParamNormalized (tag, value);
	return result;
}

Steinberg::tresult PLUGIN_API PluginController::getParamStringByValue (Steinberg::Vst::ParamID tag, Steinberg::Vst::ParamValue valueNormalized, Steinberg::Vst::String128 string)
{
	// called by host to get a string for given normalized value of a specific parameter
	// (without having to set the value!)
	return EditControllerEx1::getParamStringByValue (tag, valueNormalized, string);
}

Steinberg::tresult PLUGIN_API PluginController::getParamValueByString (Steinberg::Vst::ParamID tag, Steinberg::Vst::TChar* string, Steinberg::Vst::ParamValue& valueNormalized)
{
	// called by host to get a normalized value from a string representation of a specific parameter
	// (without having to set the value!)
	return EditControllerEx1::getParamValueByString (tag, string, valueNormalized);
}
