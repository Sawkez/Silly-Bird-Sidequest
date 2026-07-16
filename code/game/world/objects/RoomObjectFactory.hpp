#pragma once

#include "engine/GameState.hpp"
#include "engine/resource/BinaryReader.hpp"
#include "engine/world/IRoomObject.hpp"
#include "game/player/IPlayer.hpp"
#include "game/world/objects/UpgradePickup.hpp"
#include "yyjson.h"

namespace RoomObjectFactory {

enum RoomObjectType { ROOM_OBJECT_UPGRADE_PICKUP };

IRoomObject* MakeRoomObject(yyjson_val* json) {
	switch (yyjson_get_int(yyjson_obj_get(json, "type"))) {
		case ROOM_OBJECT_UPGRADE_PICKUP:
			return new UpgradePickup(GameState::GetMainRenderer(), Vector2(yyjson_obj_get(json, "position")),
									 yyjson_get_int(yyjson_obj_get(json, "upgrade")));

		default:
			return nullptr;
	}
}

IRoomObject* MakeRoomObject(BinaryReader& binary) {
	Uint16 type;
	binary.Read(2, &type);

	switch (type) {
		case ROOM_OBJECT_UPGRADE_PICKUP: {
			Uint8 upgrade;
			binary.Read(1, &upgrade);
			Vector2 position(binary);

			return new UpgradePickup(GameState::GetMainRenderer(), position, upgrade);
		}
		default:
			return nullptr;
	}

	return nullptr;
}

}  // namespace RoomObjectFactory