/*
 This file is part of SpatGRIS2.
 
 Developers: Olivier Belanger
 
 SpatGRIS2 is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 SpatGRIS2 is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with SpatGRIS2.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "ServerGrisConstants.h"

const char *DeviceName = "GRIS";
const char *ClientName = "SpatGRIS2";
const char *ClientNameSys = "system";
const char *ClientNameIgnor = "JAR::57";

#ifdef __linux__
const char *DriverNameSys = "alsa";
const bool UseOSNativeDialogBox = false;
String CURRENT_WORKING_DIR = File::getCurrentWorkingDirectory().getFullPathName();
String RESOURCES_DIR = String("/../../Resources/");
#else
const char *DriverNameSys = "coreaudio";
const bool UseOSNativeDialogBox = true;
String CURRENT_WORKING_DIR = File::getSpecialLocation(File::currentApplicationFile).getFullPathName();
String RESOURCES_DIR = String("/Contents/Resources/");
#endif

const String SplashScreenFilePath = CURRENT_WORKING_DIR + RESOURCES_DIR + "splash_screen.png";
const String DefaultPresetFilePath = CURRENT_WORKING_DIR + RESOURCES_DIR + "default_preset/default_preset.xml";
const String DefaultPresetDirectoryPath = CURRENT_WORKING_DIR + RESOURCES_DIR + "default_preset/";
const String DefaultSpeakerSetupFilePath = CURRENT_WORKING_DIR + RESOURCES_DIR + "default_preset/default_speaker_setup.xml";
const String BinauralSpeakerSetupFilePath = CURRENT_WORKING_DIR + RESOURCES_DIR + "default_preset/BINAURAL_SPEAKER_SETUP.xml";
const String StereoSpeakerSetupFilePath = CURRENT_WORKING_DIR + RESOURCES_DIR + "default_preset/STEREO_SPEAKER_SETUP.xml";
const String ServerGrisManualFilePath = CURRENT_WORKING_DIR + RESOURCES_DIR + "SpatGRIS2_2.0_Manual.pdf";
const String ServerGrisIconSmallFilePath = CURRENT_WORKING_DIR + RESOURCES_DIR + "ServerGRIS_icon_small.png";
const String HRTFFolder0Path = CURRENT_WORKING_DIR + RESOURCES_DIR + "hrtf_compact/elev" + String(0) + "/";
const String HRTFFolder40Path = CURRENT_WORKING_DIR + RESOURCES_DIR + "hrtf_compact/elev" + String(40) + "/";
const String HRTFFolder80Path = CURRENT_WORKING_DIR + RESOURCES_DIR + "hrtf_compact/elev" + String(80) + "/";

const StringArray ModeSpatString = {"DOME",  "CUBE", "BINAURAL", "STEREO"};

// Settings Jack Server
const StringArray BufferSizes = {"32", "64", "128", "256", "512", "1024", "2048"};
const StringArray RateValues = {"44100", "48000", "88200", "96000"};
const StringArray FileFormats = {"WAV", "AIFF"};
const StringArray FileConfigs = {"Multiple Mono Files", "Single Interleaved"};
const StringArray AttenuationDBs = {"0", "-12", "-24", "-36", "-48", "-60", "-72"};
const StringArray AttenuationCutoffs = {"125", "250", "500", "1000", "2000", "4000", "8000", "16000"};

const unsigned int VuMeterWidthInPixels = 22;