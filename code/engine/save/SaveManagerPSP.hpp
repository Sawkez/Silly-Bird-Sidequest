#pragma once

#include <pspdisplay.h>
#include <pspgu.h>
#include <pspkerneltypes.h>
#include <psptypes.h>
#include <psputility.h>
#include <psputility_savedata.h>
#include <string.h>

#include "engine/save/SaveData.hpp"
#include "engine/save/SaveManagerBase.hpp"

static unsigned int __attribute__((aligned(16))) list[262144];

class SaveManagerPSP : public SaveManagerBase {
   private:
	bool _visible = false;

	SceUtilitySavedataParam params;
	PspUtilitySavedataListSaveNewData newData;
	static inline char newDataTitle[24] = "New Silly Bird Savefile";

	static inline char loadList[12][20] = {"_AUTO",		"_MANUAL_0", "_MANUAL_1", "_MANUAL_2", "_MANUAL_3", "_MANUAL_4",
										   "_MANUAL_5", "_MANUAL_6", "_MANUAL_7", "_MANUAL_8", "_MANUAL_9", ""};

	static inline char saveList[11][20] = {"_MANUAL_0", "_MANUAL_1", "_MANUAL_2", "_MANUAL_3", "_MANUAL_4", "_MANUAL_5",
										   "_MANUAL_6", "_MANUAL_7", "_MANUAL_8", "_MANUAL_9", ""};

   public:
	void Init() override {
		memset(&params, 0, sizeof(SceUtilitySavedataParam));

		params.base.size = sizeof(SceUtilitySavedataParam);

		params.base.language = PSP_SYSTEMPARAM_LANGUAGE_ENGLISH;
		params.base.buttonSwap = PSP_UTILITY_ACCEPT_CROSS;
		params.base.graphicsThread = 0x11;
		params.base.accessThread = 0x13;
		params.base.fontThread = 0x12;
		params.base.soundThread = 0x10;

		params.overwrite = 1;

		strcpy(params.gameName, "SBSQ2026");
		strcpy(params.saveName, "_AUTO");
		strcpy(params.fileName, "DATA.BIN");

		strcpy(params.sfoParam.title, "Silly Bird Sidequest");
		params.sfoParam.parentalLevel = 1;

		// TODO add icon0
		params.icon0FileData.buf = NULL;
		params.icon0FileData.bufSize = 0;
		params.icon0FileData.size = 0;

		params.icon1FileData.buf = NULL;
		params.icon1FileData.bufSize = 0;
		params.icon1FileData.size = 0;

		// TODO load pic1 from mod folder
		params.pic1FileData.buf = NULL;
		params.pic1FileData.bufSize = 0;
		params.pic1FileData.size = 0;

		params.snd0FileData.buf = NULL;
		params.snd0FileData.bufSize = 0;
		params.snd0FileData.size = 0;

#if defined(_PSP_FW_VERSION) && _PSP_FW_VERSION >= 200
		strcpy(params.key, "ABCDEFGHIJKLMNO");
#endif

		memset(&newData, 0, sizeof(PspUtilitySavedataListSaveNewData));

		newData.icon0.buf = NULL;
		newData.icon0.bufSize = 0;
		newData.icon0.size = 0;
		newData.title = newDataTitle;

		params.newData = &newData;
	}

	void ConfigureParams(PspUtilitySavedataMode mode) {
		params.mode = mode;
		params.focus = (mode == PSP_UTILITY_SAVEDATA_LISTSAVE) ? PSP_UTILITY_SAVEDATA_FOCUS_FIRSTEMPTY : PSP_UTILITY_SAVEDATA_FOCUS_LATEST;
		params.saveNameList = mode == PSP_UTILITY_SAVEDATA_LISTLOAD ? loadList : saveList;

		strcpy(params.sfoParam.savedataTitle, saveData.modPath);
		strcpy(params.sfoParam.detail, mode == PSP_UTILITY_SAVEDATA_AUTOSAVE ? "Autosave" : "");

		params.dataBuf = (void*)&saveData;
		params.dataBufSize = sizeof(SaveData);
		params.dataSize = sizeof(SaveData);
	}

	void ShowSaveMenu() override {
		ConfigureParams(PSP_UTILITY_SAVEDATA_LISTSAVE);
		sceUtilitySavedataInitStart(&params);

		_visible = true;
	}

	void ShowLoadMenu() override {
		ConfigureParams(PSP_UTILITY_SAVEDATA_LISTLOAD);
		sceUtilitySavedataInitStart(&params);

		_visible = true;
	}

	void Autosave() override {
		ConfigureParams(PSP_UTILITY_SAVEDATA_AUTOSAVE);
		sceUtilitySavedataInitStart(&params);
	}

	void Autoload() override {}

	void Draw() override {
		if (_visible) {
			sceGuStart(GU_DIRECT, list);
			sceGuClearColor(0xff000d59);
			sceGuClearDepth(0);
			sceGuClear(GU_COLOR_BUFFER_BIT | GU_DEPTH_BUFFER_BIT);
			sceGuFinish();
			sceGuSync(GU_SYNC_FINISH, GU_SYNC_WHAT_DONE);
		}

		int status = sceUtilitySavedataGetStatus();

		switch (status) {
			case PSP_UTILITY_DIALOG_VISIBLE:
				sceUtilitySavedataUpdate(1);
				break;

			case PSP_UTILITY_DIALOG_QUIT:
				sceUtilitySavedataShutdownStart();

				break;

			case PSP_UTILITY_DIALOG_FINISHED:
				std::cout << "Save result: " << params.base.result << std::endl;

				if (params.mode == PSP_UTILITY_SAVEDATA_LISTLOAD &&
					params.base.result == 0) {	// 0 means loaded, 1 means cancelled, don't know what enum this is
					std::cout << "Loaded data: " << std::endl;
					std::cout << *((SaveData*)params.dataBuf) << std::endl;

					_loadedCallback();
				}
				break;
			case PSP_UTILITY_DIALOG_NONE:
				_visible = false;
				break;
		}

		if (_visible) {
			sceGuSwapBuffers();
		}
	}

	bool OverrideDrawing() const override { return _visible; }
};