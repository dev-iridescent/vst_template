#include "plugin_processor.h"
#include "plugin_cids.h"

#include "base/source/fstreamer.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"


PluginProcessor::PluginProcessor()
{
	//--- set the wanted controller for our processor
	setControllerClass(kPluginControllerUID);
}

PluginProcessor::~PluginProcessor() noexcept {}

Steinberg::tresult PLUGIN_API PluginProcessor::initialize (FUnknown* context)
{
	// Here the Plug-in will be instantiated
	//---always initialize the parent-------
	Steinberg::tresult result = AudioEffect::initialize (context);
	// if everything Ok, continue
	if (result != Steinberg::kResultOk)
	{
		return result;
	}

	//--- create Audio IO ------
	addAudioInput(STR16 ("Stereo In"), Steinberg::Vst::SpeakerArr::kStereo);
	addAudioOutput(STR16 ("Stereo Out"), Steinberg::Vst::SpeakerArr::kStereo);

	/* If you don't need an event bus, you can remove the next line */
	addEventInput(STR16 ("Event In"), 1);

	return Steinberg::kResultOk;
}

Steinberg::tresult PLUGIN_API PluginProcessor::terminate ()
{
	// Here the Plug-in will be de-instantiated, last possibility to remove some memory!
	//---do not forget to call parent ------
	return AudioEffect::terminate ();
}

Steinberg::tresult PLUGIN_API PluginProcessor::setActive (Steinberg::TBool state)
{
	//--- called when the Plug-in is enable/disable (On/Off) -----
	return AudioEffect::setActive (state);
}

Steinberg::tresult PLUGIN_API PluginProcessor::process (Steinberg::Vst::ProcessData& data)
{
    //--- Read inputs parameter changes-----------
    if (data.inputParameterChanges)
    {
        Steinberg::int32 numParamsChanged = data.inputParameterChanges->getParameterCount ();
        for (Steinberg::int32 index = 0; index < numParamsChanged; index++)
        {
            Steinberg::Vst::IParamValueQueue* paramQueue =
                data.inputParameterChanges->getParameterData (index);
            if (paramQueue)
            {
                Steinberg::Vst::ParamValue value;
                Steinberg::int32 sampleOffset;
                Steinberg::int32 numPoints = paramQueue->getPointCount();
                switch (paramQueue->getParameterId())
                {
                    case PluginParams::kBypassId:
                        if (paramQueue->getPoint (numPoints - 1, sampleOffset, value) ==
                            Steinberg::kResultTrue)
                            mBypass = (value > 0.5f);
                        break;
                }
            }
        }
    }

    //--- Process Audio---------------------
    //--- ----------------------------------
    if (data.numInputs == 0 || data.numOutputs == 0)
    {
        // nothing to do
        return Steinberg::kResultOk;
    }

    if (data.numSamples > 0)
    {
        // Process Algorithm
        // Ex: algo.process (data.inputs[0].channelBuffers32, data.outputs[0].channelBuffers32,
        // data.numSamples);
    }
    return Steinberg::kResultOk;

}

Steinberg::tresult PLUGIN_API PluginProcessor::setupProcessing(Steinberg::Vst::ProcessSetup& newSetup)
{
	//--- called before any processing ----
	return AudioEffect::setupProcessing (newSetup);
}

Steinberg::tresult PLUGIN_API PluginProcessor::canProcessSampleSize(Steinberg::int32 symbolicSampleSize)
{
	// by default kSample32 is supported
	if (symbolicSampleSize == Steinberg::Vst::kSample32)
		return Steinberg::kResultTrue;

	// disable the following comment if your processing support kSample64
	/* if (symbolicSampleSize == Vst::kSample64)
		return kResultTrue; */

	return Steinberg::kResultFalse;
}

Steinberg::tresult PLUGIN_API PluginProcessor::setState(Steinberg::IBStream* state)
{
	if (!state)
		return Steinberg::kResultFalse;

	// called when we load a preset or project, the model has to be reloaded

	Steinberg::IBStreamer streamer (state, kLittleEndian);

    Steinberg::int32 savedBypass = 0;
    if (!streamer.readInt32(savedBypass))
        return Steinberg::kResultFalse;

    mBypass = savedBypass > 0;
	return Steinberg::kResultOk;
}

Steinberg::tresult PLUGIN_API PluginProcessor::getState(Steinberg::IBStream* state)
{
	// here we need to save the model (preset or project)
	Steinberg::int32 toSaveBypass = mBypass ? 1 : 0;

    Steinberg::IBStreamer streamer (state, kLittleEndian);
    streamer.writeInt32(toSaveBypass);

	return Steinberg::kResultOk;
}
