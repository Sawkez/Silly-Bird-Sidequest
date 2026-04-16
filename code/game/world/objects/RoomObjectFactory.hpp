#pragma once

#include "engine/GameState.hpp"
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
									 Vector2(yyjson_obj_get(json, "relative_position")), yyjson_get_int(yyjson_obj_get(json, "upgrade")));

		default:
			return nullptr;
	}
}

}  // namespace RoomObjectFactory