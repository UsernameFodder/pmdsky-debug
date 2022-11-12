#ifndef HEADERS_FUNCTIONS_OVERLAY29_H_
#define HEADERS_FUNCTIONS_OVERLAY29_H_

struct dungeon* DungeonAlloc(void);
struct dungeon* GetDungeonPtrMaster(void);
void DungeonZInit(void);
void DungeonFree(void);
int RunDungeon(struct dungeon_init* dungeon_init_data, struct dungeon* dungeon);
bool EntityIsValid(struct entity* entity);
enum floor_type GetFloorType(void);
bool TryForcedLoss(bool skip_floor_end_check);
bool IsBossFight(enum fixed_room_id fixed_room_id);
bool IsCurrentFixedRoomBossFight(void);
bool FixedRoomIsSubstituteRoom(void);
bool StoryRestrictionsEnabled(void);
void FadeToBlack(void);
struct tile* GetTileAtEntity(struct entity* entity);
struct entity* SpawnTrap(enum trap_id trap_id, struct position* position, uint8_t team,
                         uint8_t flags);
struct entity* SpawnItemEntity(struct position* position);
bool CanTargetEntity(struct entity* user, struct entity* target);
bool CanTargetPosition(struct entity* monster, struct position* position);
void SubstitutePlaceholderStringTags(int string_id, struct entity* entity, undefined4 param_3);
bool UpdateMapSurveyorFlag(void);
bool ItemIsActive(struct entity* entity, enum item_id item_id);
void UpdateStatusIconFlags(struct entity* entity);
void LoadMappaFileAttributes(int quick_saved, int param_2, undefined* special_process);
bool IsOnMonsterSpawnList(enum monster_id monster_id);
enum monster_id GetMonsterIdToSpawn(int spawn_weight);
uint8_t GetMonsterLevelToSpawn(enum monster_id monster_id);
enum direction_id GetDirectionTowardsPosition(struct position* origin, struct position* target);
int GetChebyshevDistance(struct position* position_a, struct position* position_b);
bool IsPositionInSight(struct position* origin, struct position* target, bool user_has_dropeye);
struct entity* GetLeader(void);
uint8_t TickStatusTurnCounter(uint8_t* counter);
void AdvanceFrame(undefined param_1);
uint32_t GenerateDungeonRngSeed(void);
uint32_t GetDungeonRngPreseed(void);
void SetDungeonRngPreseed(uint32_t preseed);
void InitDungeonRng(uint32_t seed);
uint32_t DungeonRand16Bit(void);
uint32_t DungeonRandInt(uint32_t n);
int DungeonRandRange(int x, int y);
bool DungeonRandOutcome(int percentage);
int CalcStatusDuration(struct entity* entity, int16_t* turn_range, bool iq_skill_effects);
void DungeonRngUnsetSecondary(void);
void DungeonRngSetSecondary(int i);
void DungeonRngSetPrimary(void);
void ChangeDungeonMusic(enum music_id music_id);
void TrySwitchPlace(struct entity* user, struct entity* target);
void ClearMonsterActionFields(void* monster_action_field);
void SetMonsterActionFields(void* monster_action_field, struct action_16 action_id);
void SetActionPassTurnOrWalk(void* monster_action_field, enum monster_id monster_id);
enum action GetItemAction(int item_id);
void AddDungeonSubMenuOption(int action_id, bool enabled);
void SetActionRegularAttack(void* monster_action_field, enum direction_id direction);
void SetActionUseMoveAi(void* monster_action_field, uint8_t move_index,
                        enum direction_id direction);
void RunFractionalTurn(bool is_first_loop);
bool RunLeaderTurn(undefined param_1);
void TrySpawnMonsterAndActivatePlusMinus(void);
bool IsFloorOver(void);
void DecrementWindCounter(void);
void SetForcedLossReason(enum forced_loss_reason forced_loss_reason);
enum forced_loss_reason GetForcedLossReason(void);
void BindTrapToTile(struct tile* tile, struct entity* trap, bool is_visible);
void SpawnEnemyTrapAtPos(enum trap_id trap_id, int16_t x, int16_t y, uint8_t flags,
                         bool is_visible);
void PerformLeaderAction(void);
void ChangeLeader(void);
void ResetDamageData(struct damage_data* damage);
int GetTotalSpriteFileSize(enum monster_id monster_id);
uint16_t GetSpriteIndex(enum monster_id monster_id);
bool JoinedAtRangeCheck2Veneer(struct dungeon_id_8 joined_at);
bool FloorNumberIsEven(void);
enum monster_id GetKecleonIdToSpawnByFloor(void);
void StoreSpriteFileIndexBothGenders(enum monster_id monster_id, int file_id);
void LoadMonsterSpriteInner(enum monster_id monster_id);
void SwapMonsterWanFileIndex(int src_id, int dst_id);
void LoadMonsterSprite(enum monster_id monster_id, undefined param_2);
void DeleteMonsterSpriteFile(enum monster_id monster_id);
void DeleteAllMonsterSpriteFiles(void);
void EuFaintCheck(bool non_team_member_fainted, bool set_unk_byte);
void HandleFaint(struct entity* fainted_entity, union faint_reason faint_reason,
                 struct entity* killer);
void UpdateAiTargetPos(struct entity* monster);
void SetMonsterTypeAndAbility(struct entity* target);
void TryActivateSlowStart(void);
void TryActivateArtificialWeatherAbilities(void);
int GetMonsterApparentId(struct entity* target, enum monster_id current_id);
bool DefenderAbilityIsActive(struct entity* attacker, struct entity* defender,
                             enum ability_id ability_id, bool attacker_ability_enabled);
bool IsMonster(struct entity* entity);
void TryActivateTruant(struct entity* entity);
void RestorePpAllMovesSetFlags(struct entity* entity);
bool ShouldMonsterHeadToStairs(struct entity* entity);
bool MewSpawnCheck(enum monster_id monster_id, bool fail_if_mew);
bool ExclusiveItemEffectIsActive(struct entity* entity, enum exclusive_item_effect_id effect_id);
struct entity* GetTeamMemberWithIqSkill(enum iq_skill_id iq_skill);
bool TeamMemberHasEnabledIqSkill(enum iq_skill_id iq_skill);
bool TeamLeaderIqSkillIsEnabled(enum iq_skill_id iq_skill);
bool IsSatisfyingScenarioConditionToSpawn(enum monster_id monster_id);
bool HasLowHealth(struct entity* entity);
bool IsSpecialStoryAlly(struct monster* monster);
bool IsExperienceLocked(struct monster* monster);
void InitTeam(undefined param_1);
struct entity* SpawnMonster(struct spawned_monster_data* monster_data, bool cannot_be_asleep);
void InitTeamMember(enum monster_id, int16_t x_position, int16_t y_position,
                    struct team_member* team_member_data, undefined param_5, undefined param_6,
                    undefined param_7, undefined param_8, undefined param_9);
void InitMonster(struct monster* monster, bool flag);
void ExecuteMonsterAction(struct entity* monster);
bool HasStatusThatPreventsActing(struct entity* monster);
int CalcSpeedStage(struct entity* entity, int counter_weight);
int CalcSpeedStageWrapper(struct entity* entity);
int GetNumberOfAttacks(struct entity* entity);
void GetMonsterName(char* buffer, struct monster* target_info);
bool IsMonsterCornered(struct entity* monster);
bool CanAttackInDirection(struct entity* monster, enum direction_id direction);
bool CanAiMonsterMoveInDirection(struct entity* monster, enum direction_id direction,
                                 bool* out_monster_in_target_position);
bool ShouldMonsterRunAway(struct entity* monster);
bool ShouldMonsterRunAwayVariation(struct entity* monster, undefined param_2);
bool NoGastroAcidStatus(struct entity* entity);
bool AbilityIsActive(struct entity* entity, enum ability_id ability_id);
bool AbilityIsActiveVeneer(struct entity* entity, enum ability_id ability_id);
bool AbilityIsActiveAnyEntity(struct entity* user, enum ability_id ability_id);
bool LevitateIsActive(struct entity* entity);
bool MonsterIsType(struct entity* entity, enum type_id type_id);
bool IsTypeAffectedByGravity(struct entity* entity, enum type_id type_id);
bool HasTypeAffectedByGravity(struct entity* entity, enum type_id type_id);
bool CanSeeInvisibleMonsters(struct entity* entity);
bool HasDropeyeStatus(struct entity* entity);
bool IqSkillIsEnabled(struct entity* entity, enum iq_skill_id iq_id);
enum type_id GetMoveTypeForMonster(struct entity* entity, struct move* move);
int GetMovePower(struct entity* entity, struct move* move);
void AddExpSpecial(struct entity* attacker, struct entity* defender, int base_exp);
void EnemyEvolution(struct entity* enemy);
bool TryDecreaseLevel(struct entity* user, struct entity* target, int n_levels);
bool LevelUp(struct entity* user, struct entity* target, bool message, undefined4 param_4);
void EvolveMonster(struct entity* monster, undefined4* param_2, enum monster_id new_monster_id);
bool DisplayActions(struct entity* param_1);
bool ApplyDamage(struct entity* attacker, struct entity* defender, struct damage_data* damage_data,
                 undefined4 param_4, undefined4* param_5, union faint_reason faint_reason);
uint8_t GetSleepAnimationId(struct entity* entity);
void EndFrozenClassStatus(struct entity* user, struct entity* target, bool log);
void EndCringeClassStatus(struct entity* user, struct entity* target);
void RunMonsterAi(struct entity* monster, undefined param_2);
enum type_matchup GetTypeMatchup(struct entity* attacker, struct entity* defender,
                                 int target_type_idx, enum type_id attack_type);
void CalcDamage(struct entity* attacker, struct entity* defender, enum type_id attack_type,
                int attack_power, int crit_chance, struct damage_data* damage_out,
                int damage_mult_fp, enum move_id move_id, int param_9);
void CalcRecoilDamageFixed(struct entity* attacker, int fixed_damage, undefined4 param_3,
                           struct damage_data* damage_out, enum move_id move_id,
                           enum type_id attack_type, int16_t param_7, undefined4 message_type,
                           undefined4 param_9, undefined4 param_10);
void CalcDamageFixed(struct entity* attacker, struct entity* defender, int fixed_damage,
                     undefined4 param_4, struct damage_data* damage_out, enum type_id attack_type,
                     enum move_category move_category, int16_t param_8, undefined4 message_type,
                     undefined4 param_10, undefined4 param_11);
void CalcDamageFixedWrapper(struct entity* attacker, struct entity* defender, int fixed_damage,
                            undefined4 param_4, struct damage_data* damage_out,
                            enum type_id attack_type, enum move_category move_category,
                            int16_t param_8, undefined4 param_9, undefined4 param_10,
                            undefined4 param_11);
void CalcDamageFixedNoCategory(struct entity* attacker, struct entity* defender, int fixed_damage,
                               undefined4 param_4, struct damage_data* damage_out,
                               enum type_id attack_type, int16_t param_7, undefined4 param_8,
                               undefined4 param_9, undefined4 param_10);
void ResetDamageCalcScratchSpace(void);
bool IsRecruited(struct entity* user, struct entity* target);
void TrySpawnMonsterAndTickSpawnCounter(void);
bool AuraBowIsActive(struct entity* entity);
int ExclusiveItemOffenseBoost(struct entity* entity, int move_category_idx);
int ExclusiveItemDefenseBoost(struct entity* entity, int move_category_idx);
void TickNoSlipCap(struct entity* entity);
void TickStatusAndHealthRegen(struct entity* entity);
void InflictSleepStatusSingle(struct entity* entity, int turns);
void TryInflictSleepStatus(struct entity* user, struct entity* target, int turns, bool log_failure);
void TryInflictNightmareStatus(struct entity* user, struct entity* target, int turns);
void TryInflictNappingStatus(struct entity* user, struct entity* target, int turns);
void TryInflictYawningStatus(struct entity* user, struct entity* target, int turns);
void TryInflictSleeplessStatus(struct entity* user, struct entity* target);
bool TryInflictPausedStatus(struct entity* user, struct entity* target, int param_3, int turns,
                            bool log_failure, bool check_only);
bool TryInflictInfatuatedStatus(struct entity* user, struct entity* target, bool log_failure,
                                bool check_only);
bool TryInflictBurnStatus(struct entity* user, struct entity* target, bool special_effect,
                          bool log_failure, bool check_only);
void TryInflictBurnStatusWholeTeam(void);
bool TryInflictPoisonedStatus(struct entity* user, struct entity* target, bool log_failure,
                              bool check_only);
bool TryInflictBadlyPoisonedStatus(struct entity* user, struct entity* target, bool log_failure,
                                   bool check_only);
void TryInflictFrozenStatus(struct entity* user, struct entity* target, bool log_failure);
void TryInflictConstrictionStatus(struct entity* user, struct entity* target, int animation_id,
                                  bool log_failure);
void TryInflictShadowHoldStatus(struct entity* user, struct entity* target, bool log_failure);
void TryInflictIngrainStatus(struct entity* user, struct entity* target);
void TryInflictWrappedStatus(struct entity* user, struct entity* target);
void FreeOtherWrappedMonsters(struct entity* entity);
void TryInflictPetrifiedStatus(struct entity* user, struct entity* target);
void LowerOffensiveStat(struct entity* user, struct entity* target, int stat_idx, int16_t n_stages,
                        undefined param_5, undefined param_6);
void LowerDefensiveStat(struct entity* user, struct entity* target, int stat_idx, int16_t n_stages,
                        undefined param_5, undefined param_6);
void BoostOffensiveStat(struct entity* user, struct entity* target, int stat_idx, int16_t n_stages);
void BoostDefensiveStat(struct entity* user, struct entity* target, int stat_idx, int16_t n_stages);
void ApplyOffensiveStatMultiplier(struct entity* user, struct entity* target, int stat_idx,
                                  int multiplier, undefined param_5);
void ApplyDefensiveStatMultiplier(struct entity* user, struct entity* target, int stat_idx,
                                  int multiplier, undefined param_5);
void BoostHitChanceStat(struct entity* user, struct entity* target, int stat_idx);
void LowerHitChanceStat(struct entity* user, struct entity* target, int stat_idx, int param_4);
bool TryInflictCringeStatus(struct entity* user, struct entity* target, bool log_failure,
                            bool check_only);
bool TryInflictParalysisStatus(struct entity* user, struct entity* target, bool log_failure,
                               bool check_only);
void BoostSpeed(struct entity* user, struct entity* target, int n_stages, int turns,
                bool log_failure);
void BoostSpeedOneStage(struct entity* user, struct entity* target, int turns, bool log_failure);
void LowerSpeed(struct entity* user, struct entity* target, int n_stages, bool log_failure);
bool TrySealMove(struct entity* user, struct entity* target, bool log_failure);
void BoostOrLowerSpeed(struct entity* user, struct entity* target);
void ResetHitChanceStat(struct entity* user, struct entity* target, int stat_idx, int param_4);
bool TryActivateQuickFeet(struct entity* attacker, struct entity* defender);
bool TryInflictConfusedStatus(struct entity* user, struct entity* target, bool log_failure,
                              bool check_only);
bool TryInflictCoweringStatus(struct entity* user, struct entity* target, bool log_failure,
                              bool check_only);
bool TryRestoreHp(struct entity* user, struct entity* target, int hp_restoration);
bool TryIncreaseHp(struct entity* user, struct entity* target, int hp_restoration, int max_hp_boost,
                   bool log_failure);
void RevealItems(struct entity* user, struct entity* target);
void RevealStairs(struct entity* user, struct entity* target);
void RevealEnemies(struct entity* user, struct entity* target);
bool TryInflictLeechSeedStatus(struct entity* user, struct entity* target, bool log_failure,
                               bool check_only);
void TryInflictDestinyBond(struct entity* user, struct entity* target);
bool IsBlinded(struct entity* entity, bool check_held_item);
void RestoreMovePP(struct entity* user, struct entity* target, int pp, bool suppress_logs);
void SetReflectDamageCountdownTo4(struct entity* entity);
bool HasConditionalGroundImmunity(struct entity* entity);
int Conversion2IsActive(struct entity* entity);
int AiConsiderMove(struct ai_possible_move* ai_possible_move, struct entity* monster,
                   struct move* move);
int TryAddTargetToAiTargetList(int current_num_targets, struct move_target_and_range move_ai_range,
                               struct entity* user, struct entity* target, struct move* move,
                               bool check_all_conditions);
bool IsAiTargetEligible(struct move_target_and_range move_ai_range, struct entity* user,
                        struct entity* target, struct move* move, bool check_all_conditions);
bool IsTargetInRange(struct entity* user, struct entity* target, enum direction_id direction,
                     int n_tiles);
struct move_target_and_range GetEntityMoveTargetAndRange(struct entity* entity, struct move* move,
                                                         bool is_ai);
bool IsInSpawnList(undefined* spawn_list, enum monster_id monster_id);
int ChangeShayminForme(struct entity* entity, int forme);
void ApplyItemEffect(undefined4 param_1, undefined4 param_2, undefined4 param_3,
                     struct entity* attacker, struct entity* defender, struct item* thrown_item);
void ViolentSeedBoost(struct entity* attacker, struct entity* defender);
void ApplyGummiBoostsDungeonMode(struct entity* user, struct entity* target,
                                 enum type_id gummi_type, int random_stat_boost);
int GetMaxPpWrapper(struct move* move);
bool MoveIsNotPhysical(enum move_id move_id);
void TryPounce(struct entity* user, struct entity* target, enum direction_id direction);
void TryBlowAway(struct entity* user, struct entity* target, enum direction_id direction);
void TryWarp(struct entity* user, struct entity* target, enum warp_type warp_type,
             struct position position);
bool MoveHitCheck(struct entity* attacker, struct entity* defender, struct move* move,
                  bool use_second_accuracy);
bool IsHyperBeamVariant(struct move* move);
bool DungeonRandOutcomeUserTargetInteraction(struct entity* user, struct entity* target,
                                             int percentage);
bool DungeonRandOutcomeUserAction(struct entity* user, int percentage);
bool CanAiUseMove(struct entity* monster, int move_index, bool extra_checks);
bool CanMonsterUseMove(struct entity* monster, struct move* move, bool extra_checks);
void UpdateMovePp(struct entity* entity, bool can_consume_pp);
int LowerSshort(int x);
uint16_t GetMoveAnimationId(struct move* move, enum weather_id apparent_weather,
                            bool should_play_alternative_animation);
bool ShouldMovePlayAlternativeAnimation(struct entity* user, struct move* move);
bool DoMoveDamageWithRecoil(struct entity* attacker, struct entity* defender, struct move* move,
                            enum item_id item_id);
void ExecuteMoveEffect(undefined4* param_1, struct entity* attacker, struct move* move,
                       undefined4 param_4, undefined4 param_5);
int DealDamage(struct entity* attacker, struct entity* defender, struct move* move,
               int damage_mult_fp, enum item_id item_id);
int CalcDamageProjectile(struct entity* attacker, struct entity* defender, struct move* move,
                         int power, int damage_mult_fp, enum item_id item_id);
int CalcDamageFinal(struct entity* attacker, struct entity* defender, struct move* move,
                    struct damage_data* damage_out, union faint_reason faint_reason);
bool StatusCheckerCheck(struct entity* attacker, struct move* move);
enum weather_id GetApparentWeather(struct entity* entity);
void TryWeatherFormChange(struct entity* entity);
int DigitCount(int n);
void LoadTextureUi(void);
int DisplayNumberTextureUi(int16_t x, int16_t y, int n, int ally_mode);
int DisplayCharTextureUi(undefined* call_back_str, int16_t x, int16_t y, int char_id,
                         int16_t param_5);
void DisplayUi(void);
struct tile* GetTile(int x, int y);
struct tile* GetTileSafe(int x, int y);
bool IsFullFloorFixedRoom(void);
uint8_t GetStairsRoom(void);
enum monster_id GetRandomSpawnMonsterID(void);
bool GravityIsActive(void);
bool IsSecretBazaar(void);
bool ShouldBoostHiddenStairsSpawnChance(void);
void SetShouldBoostHiddenStairsSpawnChance(bool value);
bool IsSecretRoom(void);
struct minimap_display_data* GetMinimapData(void);
void SetMinimapDataE447(uint8_t value);
uint8_t GetMinimapDataE447(void);
void SetMinimapDataE448(uint8_t value);
bool IsSecretFloor(void);
undefined4 GetDungeonGenInfoUnk0C(void);
void LoadFixedRoomDataVeneer(void);
bool IsNormalFloor(void);
void GenerateFloor(void);
enum terrain_type GetTileTerrain(struct tile* tile);
uint32_t DungeonRand100(void);
void ClearHiddenStairs(void);
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
void SpawnStairs(uint8_t* pos, struct dungeon_generation_info* gen_info,
                 enum hidden_stairs_type hidden_stairs_type);
enum hidden_stairs_type GetHiddenStairsType(struct dungeon_generation_info* gen_info,
                                            struct floor_properties* floor_props);
void ResetHiddenStairsSpawn(void);
void LoadFixedRoomData(void);
int LoadFixedRoom(int param_1, int param_2, int param_3, undefined4 param_4);
void OpenFixedBin(void);
void CloseFixedBin(void);
bool AreOrbsAllowed(enum fixed_room_id fixed_room_id);
bool AreTileJumpsAllowed(enum fixed_room_id fixed_room_id);
bool AreTrawlOrbsAllowed(enum fixed_room_id fixed_room_id);
bool AreOrbsAllowedVeneer(enum fixed_room_id fixed_room_id);
bool AreLateGameTrapsEnabled(enum fixed_room_id fixed_room_id);
bool AreMovesEnabled(enum fixed_room_id fixed_room_id);
bool IsRoomIlluminated(enum fixed_room_id fixed_room_id);
enum monster_id GetMatchingMonsterId(enum monster_id monster_id, undefined4 param_2,
                                     undefined4 param_3);
void GenerateItemExplicit(struct item* item, enum item_id item_id, uint16_t quantity, bool sticky);
void GenerateAndSpawnItem(enum item_id item_id, int16_t x, int16_t y, uint16_t quantity,
                          bool sticky, bool check_in_bag);
bool IsHiddenStairsFloor(void);
void GenerateCleanItem(struct item* item, enum item_id item_id);
bool SpawnItem(struct position* position, struct item* item, bool flag);
bool HasHeldItem(struct entity* entity, enum item_id item_id);
void GenerateMoneyQuantity(struct item* item, int max_amount);
bool CheckTeamItemsFlags(int flags);
void GenerateItem(struct item* item, enum item_id item_id, uint16_t quantity,
                  enum gen_item_stickiness sticky_type);
bool CheckActiveChallengeRequest(void);
bool IsOutlawOrChallengeRequestFloor(void);
bool IsDestinationFloor(void);
bool IsCurrentMissionType(enum mission_type type);
bool IsCurrentMissionTypeExact(enum mission_type type, union mission_subtype subtype);
bool IsOutlawMonsterHouseFloor(void);
bool IsGoldenChamber(void);
bool IsLegendaryChallengeFloor(void);
bool IsJirachiChallengeFloor(void);
bool IsDestinationFloorWithMonster(void);
void LoadMissionMonsterSprites(void);
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
void SetTargetMonsterNotFoundFlag(bool value);
bool GetTargetMonsterNotFoundFlag(void);
bool FloorHasMissionMonster(struct mission_destination_info* mission_dst);
void GenerateMissionEggMonster(struct mission* mission);
void LogMessageByIdWithPopupCheckUser(struct entity* user, int message_id);
void LogMessageWithPopupCheckUser(struct entity* user, const char* message);
void LogMessageByIdQuiet(struct entity* user, int message_id);
void LogMessageQuiet(struct entity* user, const char* message);
void LogMessageByIdWithPopupCheckUserTarget(struct entity* user, struct entity* target,
                                            int message_id);
void LogMessageWithPopupCheckUserTarget(struct entity* user, struct entity* target,
                                        const char* message);
void LogMessageByIdQuietCheckUserTarget(struct entity* user, struct entity* target, int message_id);
void LogMessageByIdWithPopupCheckUserUnknown(struct entity* user, undefined4* param_2,
                                             int message_id);
void LogMessageByIdWithPopup(struct entity* user, int message_id);
void LogMessageWithPopup(struct entity* user, const char* message);
void LogMessage(struct entity* user, const char* message, bool show_popup);
void LogMessageById(struct entity* user, int message_id, bool show_popup);
void OpenMessageLog(undefined4 param_1, undefined4 param_2);
bool RunDungeonMode(undefined4* param_1, undefined4 param_2);
void DisplayDungeonTip(struct message_tip* message_tip, bool log);
void SetBothScreensWindowColorToDefault(void);
int GetPersonalityIndex(struct monster* monster);
void DisplayMessage(undefined4 param_1, int message_id, bool wait_for_input);
void DisplayMessage2(undefined4 param_1, int message_id, bool wait_for_input);
bool YesNoMenu(undefined param_1, int message_id, int default_option, undefined param_4);
void DisplayMessageInternal(int message_id, bool wait_for_input, undefined4 param_3,
                            undefined4 param_4, undefined4 param_5, undefined4 param_6);
void OpenMenu(undefined4 param_1, undefined4 param_2, bool param_3, undefined4 param_4);
int OthersMenuLoop(void);
undefined OthersMenu(void);
bool IsMarowakTrainingMaze(void);
void TrySpawnEnemyItemDrop(struct entity* attacker, struct entity* defender);
union faint_reason GetFaintReasonWrapper(struct move* move, enum item_id item_id);
bool DoMoveDamage(struct entity* attacker, struct entity* defender, struct move* move,
                  enum item_id item_id);
bool DoMoveIronTail(struct entity* attacker, struct entity* defender, struct move* move,
                    enum item_id item_id);
bool DoMoveYawn(struct entity* attacker, struct entity* defender, struct move* move,
                enum item_id item_id);
bool DoMoveNightmare(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveCharm(struct entity* attacker, struct entity* defender, struct move* move,
                 enum item_id item_id);
bool DoMoveEncore(struct entity* attacker, struct entity* defender, struct move* move,
                  enum item_id item_id);
bool DoMoveSuperFang(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMovePainSplit(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveTorment(struct entity* attacker, struct entity* defender, struct move* move,
                   enum item_id item_id);
bool DoMoveSwagger(struct entity* attacker, struct entity* defender, struct move* move,
                   enum item_id item_id);
bool DoMoveDamageCringe30(struct entity* attacker, struct entity* defender, struct move* move,
                          enum item_id item_id);
bool DoMoveWhirlpool(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveFakeTears(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveSpite(struct entity* attacker, struct entity* defender, struct move* move,
                 enum item_id item_id);
bool DoMoveSmokescreen(struct entity* attacker, struct entity* defender, struct move* move,
                       enum item_id item_id);
bool DoMoveFlatter(struct entity* attacker, struct entity* defender, struct move* move,
                   enum item_id item_id);
bool DoMoveWillOWisp(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveReturn(struct entity* attacker, struct entity* defender, struct move* move,
                  enum item_id item_id);
bool DoMoveFlameWheel(struct entity* attacker, struct entity* defender, struct move* move,
                      enum item_id item_id);
bool DoMoveGust(struct entity* attacker, struct entity* defender, struct move* move,
                enum item_id item_id);
bool DoMoveParalyze(struct entity* attacker, struct entity* defender, struct move* move,
                    enum item_id item_id);
bool DoMoveDamageLowerDef20(struct entity* attacker, struct entity* defender, struct move* move,
                            enum item_id item_id);
bool DoMoveBite(struct entity* attacker, struct entity* defender, struct move* move,
                enum item_id item_id);
bool DoMoveDamageParalyze20(struct entity* attacker, struct entity* defender, struct move* move,
                            enum item_id item_id);
bool DoMoveEndeavor(struct entity* attacker, struct entity* defender, struct move* move,
                    enum item_id item_id);
bool DoMoveFacade(struct entity* attacker, struct entity* defender, struct move* move,
                  enum item_id item_id);
bool DoMoveDamageLowerSpeed20(struct entity* attacker, struct entity* defender, struct move* move,
                              enum item_id item_id);
bool DoMoveBrickBreak(struct entity* attacker, struct entity* defender, struct move* move,
                      enum item_id item_id);
bool DoMoveRockTomb(struct entity* attacker, struct entity* defender, struct move* move,
                    enum item_id item_id);
bool DoMoveDamageDrain(struct entity* attacker, struct entity* defender, struct move* move,
                       enum item_id item_id);
bool DoMoveReversal(struct entity* attacker, struct entity* defender, struct move* move,
                    enum item_id item_id);
bool DoMoveSmellingSalt(struct entity* attacker, struct entity* defender, struct move* move,
                        enum item_id item_id);
bool DoMoveMetalSound(struct entity* attacker, struct entity* defender, struct move* move,
                      enum item_id item_id);
bool DoMoveTickle(struct entity* attacker, struct entity* defender, struct move* move,
                  enum item_id item_id);
bool DoMoveOutrage(struct entity* attacker, struct entity* defender, struct move* move,
                   enum item_id item_id);
bool DoMoveDamageWeightDependent(struct entity* attacker, struct entity* defender,
                                 struct move* move, enum item_id item_id);
bool DoMoveAncientPower(struct entity* attacker, struct entity* defender, struct move* move,
                        enum item_id item_id);
bool DoMoveRapidSpin(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveDamageFreeze15(struct entity* attacker, struct entity* defender, struct move* move,
                          enum item_id item_id);
bool DoMoveScaryFace(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveRockClimb(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveEarthquake(struct entity* attacker, struct entity* defender, struct move* move,
                      enum item_id item_id);
enum nature_power_variant GetNaturePowerVariant(void);
bool DoMoveNaturePower(struct entity* attacker, struct entity* defender, struct move* move,
                       enum item_id item_id);
bool DoMoveLick(struct entity* attacker, struct entity* defender, struct move* move,
                enum item_id item_id);
bool DoMoveFissure(struct entity* attacker, struct entity* defender, struct move* move,
                   enum item_id item_id);
bool DoMoveExtrasensory(struct entity* attacker, struct entity* defender, struct move* move,
                        enum item_id item_id);
bool DoMoveAbsorb(struct entity* attacker, struct entity* defender, struct move* move,
                  enum item_id item_id);
bool DoMoveSkillSwap(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveHeadbutt(struct entity* attacker, struct entity* defender, struct move* move,
                    enum item_id item_id);
bool DoMoveDoubleEdge(struct entity* attacker, struct entity* defender, struct move* move,
                      enum item_id item_id);
bool DoMoveSandAttack(struct entity* attacker, struct entity* defender, struct move* move,
                      enum item_id item_id);
bool DoMoveDamagePoison40(struct entity* attacker, struct entity* defender, struct move* move,
                          enum item_id item_id);
bool DoMoveSacredFire(struct entity* attacker, struct entity* defender, struct move* move,
                      enum item_id item_id);
bool DoMoveSheerCold(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveDamageLowerAccuracy40(struct entity* attacker, struct entity* defender,
                                 struct move* move, enum item_id item_id);
bool DoMoveTwister(struct entity* attacker, struct entity* defender, struct move* move,
                   enum item_id item_id);
bool DoMoveTwineedle(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveSeismicToss(struct entity* attacker, struct entity* defender, struct move* move,
                       enum item_id item_id);
bool DoMoveSupersonic(struct entity* attacker, struct entity* defender, struct move* move,
                      enum item_id item_id);
bool DoMoveTaunt(struct entity* attacker, struct entity* defender, struct move* move,
                 enum item_id item_id);
bool DoMoveHornDrill(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveThundershock(struct entity* attacker, struct entity* defender, struct move* move,
                        enum item_id item_id);
bool DoMoveThunderWave(struct entity* attacker, struct entity* defender, struct move* move,
                       enum item_id item_id);
bool DoMoveBlock(struct entity* attacker, struct entity* defender, struct move* move,
                 enum item_id item_id);
bool DoMovePoisonGas(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveToxic(struct entity* attacker, struct entity* defender, struct move* move,
                 enum item_id item_id);
bool DoMovePoisonFang(struct entity* attacker, struct entity* defender, struct move* move,
                      enum item_id item_id);
bool DoMovePoisonSting(struct entity* attacker, struct entity* defender, struct move* move,
                       enum item_id item_id);
bool DoMoveTriAttack(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveSwapItems(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveTripleKick(struct entity* attacker, struct entity* defender, struct move* move,
                      enum item_id item_id);
bool DoMoveMudSlap(struct entity* attacker, struct entity* defender, struct move* move,
                   enum item_id item_id);
bool DoMoveThief(struct entity* attacker, struct entity* defender, struct move* move,
                 enum item_id item_id);
bool DoMoveRolePlay(struct entity* attacker, struct entity* defender, struct move* move,
                    enum item_id item_id);
bool DoMoveLeer(struct entity* attacker, struct entity* defender, struct move* move,
                enum item_id item_id);
bool DoMoveFakeOut(struct entity* attacker, struct entity* defender, struct move* move,
                   enum item_id item_id);
bool DoMovePayDay(struct entity* attacker, struct entity* defender, struct move* move,
                  enum item_id item_id);
bool DoMoveCurse(struct entity* attacker, struct entity* defender, struct move* move,
                 enum item_id item_id);
bool DoMoveSuperpower(struct entity* attacker, struct entity* defender, struct move* move,
                      enum item_id item_id);
bool DoMoveDynamicPunch(struct entity* attacker, struct entity* defender, struct move* move,
                        enum item_id item_id);
bool DoMoveKnockOff(struct entity* attacker, struct entity* defender, struct move* move,
                    enum item_id item_id);
bool DoMoveSecretPower(struct entity* attacker, struct entity* defender, struct move* move,
                       enum item_id item_id);
bool DoMoveDizzyPunch(struct entity* attacker, struct entity* defender, struct move* move,
                      enum item_id item_id);
bool DoMoveImprison(struct entity* attacker, struct entity* defender, struct move* move,
                    enum item_id item_id);
bool DoMoveFeatherDance(struct entity* attacker, struct entity* defender, struct move* move,
                        enum item_id item_id);
bool DoMoveBeatUp(struct entity* attacker, struct entity* defender, struct move* move,
                  enum item_id item_id);
bool DoMoveBlastBurn(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveCrushClaw(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveBlazeKick(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMovePresent(struct entity* attacker, struct entity* defender, struct move* move,
                   enum item_id item_id);
bool DoMoveEruption(struct entity* attacker, struct entity* defender, struct move* move,
                    enum item_id item_id);
bool DoMovePoisonTail(struct entity* attacker, struct entity* defender, struct move* move,
                      enum item_id item_id);
bool DoMoveRoar(struct entity* attacker, struct entity* defender, struct move* move,
                enum item_id item_id);
bool DoMoveDamageConstrict10(struct entity* attacker, struct entity* defender, struct move* move,
                             enum item_id item_id);
bool DoMoveWrap(struct entity* attacker, struct entity* defender, struct move* move,
                enum item_id item_id);
bool DoMoveMagnitude(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveMistBall(struct entity* attacker, struct entity* defender, struct move* move,
                    enum item_id item_id);
bool DoMoveDestinyBond(struct entity* attacker, struct entity* defender, struct move* move,
                       enum item_id item_id);
bool DoMoveHiddenPower(struct entity* attacker, struct entity* defender, struct move* move,
                       enum item_id item_id);
bool DoMoveAttract(struct entity* attacker, struct entity* defender, struct move* move,
                   enum item_id item_id);
bool DoMoveCopycat(struct entity* attacker, struct entity* defender, struct move* move,
                   enum item_id item_id);
bool DoMoveFrustration(struct entity* attacker, struct entity* defender, struct move* move,
                       enum item_id item_id);
bool DoMoveLeechSeed(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveDreamEater(struct entity* attacker, struct entity* defender, struct move* move,
                      enum item_id item_id);
bool DoMoveDragonRage(struct entity* attacker, struct entity* defender, struct move* move,
                      enum item_id item_id);
bool DoMoveDamageLowerSpecialDefense50(struct entity* attacker, struct entity* defender,
                                       struct move* move, enum item_id item_id);
bool DoMoveFling(struct entity* attacker, struct entity* defender, struct move* move,
                 enum item_id item_id);
bool DoHammerArm(struct entity* attacker, struct entity* defender, struct move* move,
                 enum item_id item_id);
bool DoMoveGastroAcid(struct entity* attacker, struct entity* defender, struct move* move,
                      enum item_id item_id);
bool DoMoveCloseCombat(struct entity* attacker, struct entity* defender, struct move* move,
                       enum item_id item_id);
bool DoMoveGuardSwap(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveThunderFang(struct entity* attacker, struct entity* defender, struct move* move,
                       enum item_id item_id);
bool DoMoveDefog(struct entity* attacker, struct entity* defender, struct move* move,
                 enum item_id item_id);
bool DoMoveTrumpCard(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveIceFang(struct entity* attacker, struct entity* defender, struct move* move,
                   enum item_id item_id);
bool DoMovePsychoShift(struct entity* attacker, struct entity* defender, struct move* move,
                       enum item_id item_id);
bool DoMoveEmbargo(struct entity* attacker, struct entity* defender, struct move* move,
                   enum item_id item_id);
bool DoMoveBrine(struct entity* attacker, struct entity* defender, struct move* move,
                 enum item_id item_id);
bool DoMoveNaturalGift(struct entity* attacker, struct entity* defender, struct move* move,
                       enum item_id item_id);
bool DoMoveGyroBall(struct entity* attacker, struct entity* defender, struct move* move,
                    enum item_id item_id);
bool DoMoveChargeBeam(struct entity* attacker, struct entity* defender, struct move* move,
                      enum item_id item_id);
bool DoMoveDamageEatItem(struct entity* attacker, struct entity* defender, struct move* move,
                         enum item_id item_id);
bool DoMoveLastResort(struct entity* attacker, struct entity* defender, struct move* move,
                      enum item_id item_id);
bool DoMoveDamageHpDependent(struct entity* attacker, struct entity* defender, struct move* move,
                             enum item_id item_id);
bool DoMoveHeartSwap(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMovePowerSwap(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveFeint(struct entity* attacker, struct entity* defender, struct move* move,
                 enum item_id item_id);
bool DoMoveFlareBlitz(struct entity* attacker, struct entity* defender, struct move* move,
                      enum item_id item_id);
bool DoMoveFireFang(struct entity* attacker, struct entity* defender, struct move* move,
                    enum item_id item_id);
bool DoMoveMiracleEye(struct entity* attacker, struct entity* defender, struct move* move,
                      enum item_id item_id);
bool DoMoveWakeUpSlap(struct entity* attacker, struct entity* defender, struct move* move,
                      enum item_id item_id);
bool DoMoveHeadSmash(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveCaptivate(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveDamageInlined(struct entity* attacker, struct entity* defender, struct move* move,
                         enum item_id item_id);
void GenerateStandardItem(struct item* item, enum item_id item_id,
                          enum gen_item_stickiness sticky_type);
void SpawnEnemyItemDropWrapper(struct entity* entity, struct position* pos, struct item* item,
                               undefined4 param_4);
void SpawnEnemyItemDrop(struct entity* entity, struct entity* item_entity, struct item* item,
                        int param_4, int16_t* dir_xy, undefined param_6);
bool TryGenerateUnownStoneDrop(struct item* item, enum monster_id monster_id);

#endif
