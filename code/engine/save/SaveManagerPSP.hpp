#pragma once

#include <pspdisplay.h>
#include <pspgu.h>
#include <pspkerneltypes.h>
#include <psptypes.h>
#include <psputility.h>
#include <psputility_savedata.h>
#include <string.h>

#include "engine/save/ISaveManager.hpp"

class SaveManagerPSP : public ISaveManager {
   private:
	bool _visible = false;

	SceUtilitySavedataParam params;
	std::string data = "i eat bugs ! yippee ooga booga";
	PspUtilitySavedataListSaveNewData newData;
	char newDataTitle[12] = "No bitches?";

	char nameMultiple[5][20] = {"0000", "0001", ""};

	void ShowMenu() override {
		params.base.size = sizeof(SceUtilitySavedataParam);

		params.base.language = PSP_SYSTEMPARAM_LANGUAGE_ENGLISH;
		params.base.buttonSwap = PSP_UTILITY_ACCEPT_CROSS;
		params.base.graphicsThread = 0x11;
		params.base.accessThread = 0x13;
		params.base.fontThread = 0x12;
		params.base.soundThread = 0x10;

		params.mode = PSP_UTILITY_SAVEDATA_LISTSAVE;
		params.overwrite = 1;
		params.focus = PSP_UTILITY_SAVEDATA_FOCUS_LATEST;

		strcpy(params.gameName, "SBSidequest");
		strcpy(params.saveName, "0000");
		params.saveNameList = nameMultiple;

		strcpy(params.fileName, "poo.pee");
		params.dataBuf = data.data();
		params.dataBufSize = data.length() + 1;
		params.dataSize = data.length() + 1;

		strcpy(params.sfoParam.title, "Silly Bird Sidequest");
		strcpy(params.sfoParam.savedataTitle, "Mod name");
		strcpy(params.sfoParam.detail, "you SUCK at this game ngl 🧟");
		params.sfoParam.parentalLevel = 1;

		params.icon0FileData.buf = NULL;
		params.icon0FileData.bufSize = 0;
		params.icon0FileData.size = 0;

		params.icon1FileData.buf = NULL;
		params.icon1FileData.bufSize = 0;
		params.icon1FileData.size = 0;

		params.pic1FileData.buf = NULL;
		params.pic1FileData.bufSize = 0;
		params.pic1FileData.size = 0;

		params.snd0FileData.buf = NULL;
		params.snd0FileData.bufSize = 0;
		params.snd0FileData.size = 0;

		newData.title = newDataTitle;
		params.newData = &newData;

		sceUtilitySavedataInitStart(&params);

		_visible = true;
	}

	void Process() override {
		if (!_visible) return;
		switch (sceUtilitySavedataGetStatus()) {
			case PSP_UTILITY_DIALOG_VISIBLE:
				sceUtilitySavedataUpdate(1);
				break;

			case PSP_UTILITY_DIALOG_QUIT:
				sceUtilitySavedataShutdownStart();
				break;

				// case PSP_UTILITY_DIALOG_NONE:
				//	return;
		}
	}
};