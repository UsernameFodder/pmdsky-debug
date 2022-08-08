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
bool FixedRoomIsSubstituteRoom(void);
bool StoryRestrictionsEnabled(void);
void FadeToBlack(void);
struct tile* GetTileAtEntity(struct entity* entity);
bool CanTargetEntity(struct entity* user, struct entity* target);
bool CanTargetPosition(struct entity* monster, struct position* position);
void SubstitutePlaceholderStringTags(int string_id, struct entity* entity, undefined4 param_3);
bool UpdateMapSurveyorFlag(void);
bool ItemIsActive(struct entity* entity, enum item_id item_id);
void UpdateStatusIconFlags(struct entity* entity);
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
int CalcStatusDuration(struct entity* entity, uint16_t* turn_range, bool iq_skill_effects);
void DungeonRngUnsetSecondary(void);
void DungeonRngSetSecondary(int i);
void DungeonRngSetPrimary(void);
void TrySwitchPlace(struct entity* user, struct entity* target);
void ClearMonsterActionFields(void* monster_action_field);
void SetMonsterActionFields(void* monster_action_field, struct action_16 action_id);
void SetActionPassTurnOrWalk(void* monster_action_field, enum monster_id monster_id);
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
void ResetDamageDesc(undefined4* damage_desc);
uint16_t GetSpriteIndex(enum monster_id monster_id);
bool FloorNumberIsEven(void);
void EuFaintCheck(bool non_team_member_fainted, bool set_unk_byte);
void HandleFaint(struct entity* fainted_entity, int faint_reason, struct entity* killer);
void UpdateAiTargetPos(struct entity* monster);
enum monster_id GetKecleonIdToSpawnByFloor(void);
void LoadMonsterSprite(enum monster_id monster_id, undefined param_2);
void TryActivateSlowStart(void);
void TryActivateArtificialWeatherAbilities(void);
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
bool HasLowHealth(struct entity* entity);
bool IsSpecialStoryAlly(struct monster* monster);
bool IsExperienceLocked(struct monster* monster);
void InitTeam(undefined param_1);
struct entity* SpawnMonster(struct spawned_monster_data* monster_data, bool cannot_be_asleep);
void InitTeamMember(enum monster_id, enum type_id type_1, enum type_id type_2,
                    struct team_member* team_member_data, undefined param_5, undefined param_6,
                    undefined param_7, undefined param_8, undefined param_9);
void ExecuteMonsterAction(struct entity* monster);
bool HasStatusThatPreventsActing(struct entity* monster);
int CalcSpeedStage(struct entity* entity, int counter_weight);
int CalcSpeedStageWrapper(struct entity* entity);
int GetNumberOfAttacks(struct entity* entity);
bool IsMonsterCornered(struct entity* monster);
bool CanAttackInDirection(struct entity* monster, enum direction_id direction);
bool CanAiMonsterMoveInDirection(struct entity* monster, enum direction_id direction,
                                 struct entity* out_monster_in_target_position);
bool ShouldMonsterRunAway(struct entity* monster);
bool ShouldMonsterRunAwayVariation(struct entity* monster, undefined param_2);
bool NoGastroAcidStatus(struct entity* entity);
bool AbilityIsActive(struct entity* entity, enum ability_id ability_id);
bool LevitateIsActive(struct entity* entity);
bool MonsterIsType(struct entity* entity, enum type_id type_id);
bool CanSeeInvisibleMonsters(struct entity* entity);
bool HasDropeyeStatus(struct entity* entity);
bool IqSkillIsEnabled(struct entity* entity, enum iq_skill_id iq_id);
enum type_id GetMoveTypeForMonster(struct entity* entity, struct move* move);
int GetMovePower(struct entity* entity, struct move* move);
void AddExpSpecial(struct entity* attacker, struct entity* defender, int base_exp);
void EnemyEvolution(struct entity* enemy);
void EvolveMonster(struct entity* monster, undefined4* param_2, enum monster_id new_monster_id);
bool DisplayActions(struct entity* param_1);
bool ApplyDamage(struct entity* attacker, struct entity* defender, struct damage_data* damage_data,
                 undefined4 param_4, undefined4* param_5, undefined4* param_6);
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
bool TryIncreaseHp(struct entity* user, struct entity* target, int hp_restoration, int max_hp_boost,
                   bool log_failure);
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
bool DealDamageWithRecoil(struct entity* attacker, struct entity* defender, struct move* move,
                          enum item_id item_id);
void ExecuteMoveEffect(undefined4* param_1, struct entity* attacker, struct move* move,
                       undefined4 param_4, undefined4 param_5);
int DealDamage(struct entity* attacker, struct entity* defender, struct move* move,
               int damage_mult_fp, enum item_id item_id);
void CalcDamageProjectile(struct entity* attacker, struct entity* defender, struct move* move,
                          int power, undefined4 param_5, undefined4 param_6);
int CalcDamageFinal(struct entity* attacker, struct entity* defender, struct move* move,
                    undefined4 param_4, undefined4* param_5);
bool StatusCheckerCheck(struct entity* attacker, struct move* move);
enum weather_id GetApparentWeather(struct entity* entity);
void TryWeatherFormChange(struct entity* entity);
struct tile* GetTile(int x, int y);
struct tile* GetTileSafe(int x, int y);
uint8_t GetStairsRoom(void);
bool GravityIsActive(void);
bool IsSecretBazaar(void);
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
bool IsOutlawOrChallengeRequestFloor(void);
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
void DisplayMessage(undefined4 param_1, int message_id, bool wait_for_input);
void DisplayMessage2(undefined4 param_1, int message_id, bool wait_for_input);
void DisplayMessageInternal(int message_id, bool wait_for_input, undefined4 param_3,
                            undefined4 param_4, undefined4 param_5, undefined4 param_6);

#endif
