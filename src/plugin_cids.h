#pragma once

#include "pluginterfaces/base/funknown.h"
#include "pluginterfaces/vst/vsttypes.h"

#define PluginVST3Category "Fx"

enum PluginParams : Steinberg::Vst::ParamID
{
    kBypassId = 100
};

static const Steinberg::FUID kPluginProcessorUID(0x0, 0x0, 0x0, 0x0);
static const Steinberg::FUID kPluginControllerUID(0x0, 0x0, 0x0, 0x1);
