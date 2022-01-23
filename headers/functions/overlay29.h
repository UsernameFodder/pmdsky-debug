#ifndef HEADERS_FUNCTIONS_OVERLAY29_H_
#define HEADERS_FUNCTIONS_OVERLAY29_H_

struct dungeon* DungeonAlloc(void);
struct dungeon* GetDungeonPtrMaster(void);
void DungeonZInit(void);
void DungeonFree(void);
int InitializeDungeon(undefined* dungeon_data, struct dungeon* dungeon);
enum floor_type GetFloorType(void);
bool FixedRoomIsSubstituteRoom(void);
uint32_t GenerateRngSeed(void);
uint32_t GetRngPreseed(void);
void SetRngPreseed(uint32_t preseed);
void InitRng(uint32_t seed);
uint32_t Rand16Bit(void);
uint32_t RandInt(uint32_t n);
int RandRange(int x, int y);
int CalcStatusDuration(struct entity* entity, uint16_t* turn_range, bool iq_skill_effects);
void RngUnsetSecondary(void);
void RngSetSecondary(int i);
void RngSetPrimary(void);
bool EntityIsValid(struct entity* entity);
void ResetDamageDesc(undefined4* damage_desc);
bool FloorNumberIsEven(void);
bool ItemIsActive(struct entity* entity, enum item_id item_id);
bool IsMonster(struct entity* entity);
bool NoGastroAcidStatus(struct entity* entity);
bool AbilityIsActive(struct entity* entity, enum ability_id ability_id);
bool MonsterIsType(struct entity* entity, enum type_id type_id);
bool IqSkillIsEnabled(struct entity* entity, enum iq_skill_id iq_id);
int GetMovePower(struct entity* entity, struct move* move);
bool DefenderAbilityIsActive(struct entity* attacker, struct entity* defender,
                             enum ability_id ability_id, bool attacker_ability_enabled);
bool ExclusiveItemEffectIsActive(struct entity* entity, enum exclusive_item_effect_id effect_id);
void CalcDamage(struct entity* attacker, struct entity* defender, enum type_id attack_type,
                int attack_power, int crit_chance, undefined4* damage_out, int param_7,
                enum move_id move_id, int param_9);
void CalcDamageFixed(struct entity* attacker, struct entity* defender, int fixed_damage,
                     undefined4 param_4, undefined4* damage_out, enum type_id attack_type,
                     enum move_category move_category, int16_t param_8, undefined4 param_9,
                     undefined4 param_10, undefined4 param_11);
void CalcDamageFixedWrapper(struct entity* attacker, struct entity* defender, int fixed_damage,
                            undefined4 param_4, undefined4* damage_out, enum type_id attack_type,
                            enum move_category move_category, int16_t param_8, undefined4 param_9,
                            undefined4 param_10, undefined4 param_11);
void CalcDamageFixedNoCategory(struct entity* attacker, struct entity* defender, int fixed_damage,
                               undefined4 param_4, undefined4* damage_out, enum type_id attack_type,
                               int16_t param_7, undefined4 param_8, undefined4 param_9,
                               undefined4 param_10);
void ResetDamageCalcScratchSpace(void);
bool AuraBowIsActive(struct entity* entity);
int ExclusiveItemOffenseBoost(struct entity* entity, int move_category_idx);
int ExclusiveItemDefenseBoost(struct entity* entity, int move_category_idx);
bool PptrIsValid(void** pptr);
void BoostStat(struct entity* attacker, struct entity* defender, int stat_idx, int16_t boost);
void BoostSpeed(struct entity* attacker, struct entity* defender, undefined4 param_3,
                undefined4 param_4, undefined4 param_5);
void BoostSpeedWrapper(struct entity* attacker, struct entity* defender, undefined4 param_3,
                       undefined4 param_4);
bool TryActivateQuickFeet(struct entity* attacker, struct entity* defender);
bool TeamExclusiveItemEffectIsActive(struct entity* entity,
                                     enum exclusive_item_effect_id effect_id);
void ApplyItemEffect(undefined4 param_1, undefined4 param_2, undefined4 param_3,
                     struct entity* attacker, struct entity* defender, struct item* thrown_item);
void ViolentSeedBoost(struct entity* attacker, struct entity* defender);
void ApplyGummiIqBoost(struct entity* attacker, struct entity* defender, enum type_id gummi_type,
                       int base_boost);
int GetMaxPpWrapper(struct move* move);
bool MoveIsNotPhysical(enum move_id move_id);
void UpdateMovePp(struct entity* entity, bool can_consume_pp);
int LowerSshort(int x);
void CalcDamageProjectile(struct entity* attacker, struct entity* defender, struct move* move,
                          int power, undefined4 param_5, undefined4 param_6);
enum weather_id GetApparentWeather(struct entity* entity);
struct tile* GetTile(int x, int y);
bool IsSecretBazaar(void);
bool IsSecretRoom(void);
void LoadFixedRoomDataWrapper(void);
bool IsNormalFloor(void);
void GenerateFloor(void);
enum terrain_type GetTileTerrain(struct tile* tile);
uint32_t Rand100(void);
void FlagHallwayJunctions(int x0, int y0, int x1, int y1);
void GenerateStandardFloor(int grid_size_x, int grid_size_y, struct floor_properties* floor_props);
void GenerateOuterRingFloor(struct floor_properties* floor_props);
void GenerateCrossroadsFloor(struct floor_properties* floor_props);
void GenerateLineFloor(struct floor_properties* floor_props);
void GenerateCrossFloor(struct floor_properties* floor_props);
void GenerateBeetleFloor(struct floor_properties* floor_props);
void MergeRoomsVertically(int x, int y0, int dy, struct dungeon_grid_cell* grid);
void GenerateOuterRoomsFloor(int grid_size_x, int grid_size_y,
                             struct floor_properties* floor_props);
bool IsNotFullFloorFixedRoom(enum fixed_room_id fixed_room_id);
bool GenerateFixedRoom(enum fixed_room_id fixed_room_id, struct floor_properties* floor_props);
void GenerateOneRoomMonsterHouseFloor(void);
void GenerateTwoRoomsWithMonsterHouseFloor(void);
void GenerateExtraHallways(struct dungeon_grid_cell* grid, int grid_size_x, int grid_size_y,
                           int n_extra_hallways);
void GetGridPositions(int* grid_starts_x, int* grid_starts_y, int grid_size_x, int grid_size_y);
void InitDungeonGrid(struct dungeon_grid_cell* grid, int grid_size_x, int grid_size_y);
void AssignRooms(struct dungeon_grid_cell* grid, int grid_size_x, int grid_size_y, int n_rooms);
void CreateRoomsAndAnchors(struct dungeon_grid_cell* grid, int grid_size_x, int grid_size_y,
                           int* grid_starts_x, int* grid_starts_y, uint32_t room_flags);
void GenerateSecondaryStructures(struct dungeon_grid_cell* grid, int grid_size_x, int grid_size_y);
void AssignGridCellConnections(struct dungeon_grid_cell* grid, int grid_size_x, int grid_size_y,
                               int cursor_x, int cursor_y, struct floor_properties* floor_props);
void CreateGridCellConnections(struct dungeon_grid_cell* grid, int grid_size_x, int grid_size_y,
                               int* grid_starts_x, int* grid_starts_y, bool disable_room_merging);
void GenerateRoomImperfections(struct dungeon_grid_cell* grid, int grid_size_x, int grid_size_y);
void CreateHallway(int x0, int y0, int x1, int y1, bool vertical, int x_mid, int y_mid);
void EnsureConnectedGrid(struct dungeon_grid_cell* grid, int grid_size_x, int grid_size_y,
                         int* grid_starts_x, int* grid_starts_y);
void SetTerrainObstacleChecked(struct tile* tile, bool use_secondary_terrain, uint8_t room);
void FinalizeJunctions(void);
void GenerateKecleonShop(struct dungeon_grid_cell* grid, int grid_size_x, int grid_size_y,
                         int spawn_chance);
void GenerateMonsterHouse(struct dungeon_grid_cell* grid, int grid_size_x, int grid_size_y,
                          int spawn_chance);
void GenerateMazeRoom(struct dungeon_grid_cell* grid, int grid_size_x, int grid_size_y,
                      int spawn_chance);
void GenerateMaze(struct dungeon_grid_cell* grid_cell, bool use_secondary_terrain);
void GenerateMazeLine(int x0, int y0, int xmin, int ymin, int xmax, int ymax,
                      bool use_secondary_terrain, uint8_t room);
void SetSpawnFlag5(struct dungeon_grid_cell* grid_cell);
bool IsNextToHallway(int x, int y);
void ResolveInvalidSpawns(void);
void ConvertSecondaryTerrainToChasms(void);
void EnsureImpassableTilesAreWalls(void);
void InitializeTile(struct tile* tile);
void ResetFloor(void);
bool PosIsOutOfBounds(int x, int y);
void ShuffleSpawnPositions(struct spawn_position* spawn_positions, int n_spawn_positions);
void SpawnNonEnemies(struct floor_properties* floor_props, bool empty_monster_house);
void SpawnEnemies(struct floor_properties* floor_props, bool empty_monster_house);
void SetSecondaryTerrainOnWall(struct tile* tile);
void GenerateSecondaryTerrainFormations(uint8_t test_flag, struct floor_properties* floor_props);
bool StairsAlwaysReachable(int x_stairs, int y_stairs, bool mark_unreachable);
void ConvertWallsToChasms(void);
void ResetInnerBoundaryTileRows(void);
void SpawnStairs(uint8_t* pos, struct dungeon_generation_info* gen_info, bool hidden_stairs);
void LoadFixedRoomData(void);
bool IsHiddenStairsFloor(void);
bool HasHeldItem(struct entity* entity, enum item_id item_id);
bool IsCurrentMissionType(enum mission_type type);
bool IsCurrentMissionTypeExact(enum mission_type type, union mission_subtype subtype);
bool IsOutlawMonsterHouseFloor(void);
bool IsGoldenChamber(void);
bool IsLegendaryChallengeFloor(void);
bool IsJirachiChallengeFloor(void);
bool IsDestinationFloorWithMonster(void);
bool MissionTargetEnemyIsDefeated(void);
void SetMissionTargetEnemyDefeated(bool defeated);
bool IsDestinationFloorWithFixedRoom(void);
enum item_id GetItemToRetrieve(void);
enum item_id GetItemToDeliver(void);
enum item_id GetSpecialTargetItem(void);
bool IsDestinationFloorWithItem(void);
bool IsDestinationFloorWithHiddenOutlaw(void);
bool IsDestinationFloorWithFleeingOutlaw(void);
enum monster_id GetMissionTargetEnemy(void);
enum monster_id GetMissionEnemyMinionGroup(int i);
bool FloorHasMissionMonster(struct mission_destination_info* mission_dst);
bool RunDungeonMode(undefined4* param_1, undefined4 param_2);

#endif
