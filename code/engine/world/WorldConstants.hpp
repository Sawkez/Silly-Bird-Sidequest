#pragma once

namespace WorldConstants {
inline constexpr int TILE_SIZE = 8;
inline constexpr float TILE_SIZE_F = float(TILE_SIZE);
inline constexpr int TILE_TEXTURE_SIZE = 14;
inline constexpr float TILE_TEXTURE_SIZE_F = float(TILE_TEXTURE_SIZE);
inline constexpr int TILE_PERSPECTIVE_OVERLAP = (TILE_TEXTURE_SIZE - TILE_SIZE) / 2;
inline constexpr float TILE_PERSPECTIVE_OVERLAP_F = float(TILE_PERSPECTIVE_OVERLAP);
}  // namespace WorldConstants