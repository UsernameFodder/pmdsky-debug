#ifndef HEADERS_FUNCTIONS_OVERLAY29_H_
#define HEADERS_FUNCTIONS_OVERLAY29_H_

#include "overlay29/move_effects.h"

struct rgba* GetWeatherColorTable(enum weather_id);
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
bool IsMarowakTrainingMaze(void);
bool FixedRoomIsSubstituteRoom(void);
bool StoryRestrictionsEnabled(void);
int GetScenarioBalanceVeneer(void);
void FadeToBlack(void);
bool CheckTouchscreenArea(int x1, int y1, int x2, int y2);
struct trap* GetTrapInfo(struct entity* trap_entity);
struct item* GetItemInfo(struct entity* item_entity);
struct tile* GetTileAtEntity(struct entity* entity);
void UpdateEntityPixelPos(struct entity* entity, struct pixel_position* pixel_pos);
struct entity* CreateEnemyEntity(enum monster_id monster_id);
struct entity* SpawnTrap(enum trap_id trap_id, struct position* position, uint8_t team,
                         uint8_t flags);
struct entity* SpawnItemEntity(struct position* position);
bool ShouldMinimapDisplayEntity(struct entity* entity);
bool ShouldDisplayEntity(struct entity* entity, undefined param_2);
bool ShouldDisplayEntityWrapper(struct entity* entity);
bool CanSeeTarget(struct entity* user, struct entity* target);
bool CanTargetEntity(struct entity* user, struct entity* target);
bool CanTargetPosition(struct entity* monster, struct position* position);
int GetTeamMemberIndex(struct entity* monster);
void SubstitutePlaceholderStringTags(int string_id, struct entity* entity, undefined4 param_3);
bool UpdateMapSurveyorFlag(void);
void PointCameraToMonster(struct entity* entity, undefined param_2);
void UpdateCamera(undefined param_1);
bool ItemIsActive(struct entity* entity, enum item_id item_id);
int GetVisibilityRange(void);
int PlayEffectAnimationEntity(struct entity* entity, int effect_id, bool play_now, int param_4,
                              int param_5, undefined param_6, int param_7, undefined2* param_8);
int PlayEffectAnimationPos(struct position* pos, int effect_id, bool play_now);
int PlayEffectAnimationPixelPos(struct pixel_position* pixel_pos, int effect_id, bool play_now);
void AnimationDelayOrSomething(undefined param_1);
void UpdateStatusIconFlags(struct entity* entity);
void PlayEffectAnimation0x171Full(struct entity* entity);
void PlayEffectAnimation0x171(struct entity* entity);
void ShowPpRestoreEffect(struct entity* entity);
void PlayEffectAnimation0x1A9(struct entity* entity);
void PlayEffectAnimation0x18E(struct entity* entity);
void LoadMappaFileAttributes(int quick_saved, bool disable_monsters, undefined* special_process);
enum item_id GetItemIdToSpawn(enum item_list_type item_list);
int MonsterSpawnListPartialCopy(struct monster_spawn_entry* buffer, int current_buffer_entries);
bool IsOnMonsterSpawnList(enum monster_id monster_id);
enum monster_id GetMonsterIdToSpawn(int spawn_weight);
uint8_t GetMonsterLevelToSpawn(enum monster_id monster_id);
enum direction_id GetDirectionTowardsPosition(struct position* origin, struct position* target);
int GetChebyshevDistance(struct position* position_a, struct position* position_b);
bool IsPositionActuallyInSight(struct position* origin, struct position* target,
                               bool user_has_dropeye);
bool IsPositionInSight(struct position* origin, struct position* target, bool user_has_dropeye);
struct entity* GetLeader(void);
struct monster* GetLeaderMonster(void);
bool FindNearbyUnoccupiedTile(struct position* pos_out, struct position* origin,
                              struct position* search_list, int search_list_len, bool random_room);
bool FindClosestUnoccupiedTileWithin2(struct position* pos_out, struct position* origin,
                                      bool random_room);
bool FindFarthestUnoccupiedTileWithin2(struct position* pos_out, struct position* origin,
                                       bool random_room);
bool FindUnoccupiedTileWithin3(struct position* pos_out, struct position* origin, bool random_room);
uint8_t TickStatusTurnCounter(uint8_t* counter);
void AdvanceFrame(undefined param_1);
void SetDungeonRngPreseed23Bit(uint32_t preseed23);
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
enum music_id MusicTableIdxToMusicId(int music_table_idx);
void ChangeDungeonMusic(enum music_id music_id);
void TrySwitchPlace(struct entity* user, struct entity* target);
void SetLeaderActionFields(enum action action_id);
void ClearMonsterActionFields(struct action_data* monster_action);
void SetMonsterActionFields(struct action_data* monster_action, enum action action_id);
void SetActionPassTurnOrWalk(struct action_data* monster_action, enum monster_id monster_id);
struct item* GetItemToUseByIndex(struct entity* entity, union item_index item_index);
struct item* GetItemToUse(struct entity* entity, int param_index, undefined param_3);
enum action GetItemAction(enum item_id item_id);
void RemoveUsedItem(struct entity* entity, int param_index);
void AddDungeonSubMenuOption(int action_id, bool enabled);
void DisableDungeonSubMenuOption(int action_id);
void SetActionRegularAttack(struct action_data* monster_action, enum direction_id direction);
void SetActionUseMovePlayer(struct action_data* monster_action, uint8_t entity_index,
                            uint8_t move_index);
void SetActionUseMoveAi(struct action_data* monster_action, uint8_t move_index,
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
void PrepareTrapperTrap(struct entity* entity, enum trap_id trap_id, uint8_t team);
bool TrySpawnTrap(struct position* pos, enum trap_id trap_id, uint8_t team, bool visible);
bool TrySpawnTrapperTrap(struct entity* entity);
void TryTriggerTrap(struct entity* entity, struct position* pos, undefined param_3,
                    undefined param_4);
void ApplyMudTrapEffect(struct entity* attacker, struct entity* defender);
void ApplyStickyTrapEffect(struct entity* attacker, struct entity* defender);
void ApplyGrimyTrapEffect(struct entity* attacker, struct entity* defender);
void ApplyPitfallTrapEffect(struct entity* attacker, struct entity* defender, struct tile* tile,
                            bool grate_stay_intact);
void ApplySummonTrapEffect(struct entity* monster, struct position* pos);
void ApplyPpZeroTrapEffect(struct entity* attacker, struct entity* defender);
void ApplyPokemonTrapEffect(struct entity* monster, struct position* pos);
void ApplyTripTrapEffect(struct entity* attacker, struct entity* defender);
void ApplyStealthRockTrapEffect(struct entity* attacker, struct entity* defender);
void ApplyToxicSpikesTrapEffect(struct entity* attacker, struct entity* defender);
bool ApplyRandomTrapEffect(struct trap* trap, struct entity* user, struct entity* target,
                           struct tile* tile, struct position* pos);
void ApplyGrudgeTrapEffect(struct entity* monster, struct position* pos);
bool ApplyTrapEffect(struct trap* trap, struct entity* user, struct entity* target,
                     struct tile* tile, struct position* pos, enum trap_id, bool random_trap);
void RevealTrapsNearby(struct entity* monster);
bool ShouldRunMonsterAi(struct entity* monster);
bool DebugRecruitingEnabled(void);
void TryActivateIqBooster(void);
bool IsSecretBazaarNpcBehavior(enum monster_behavior behavior);
struct action_16* GetLeaderAction(void);
void GetEntityTouchscreenArea(struct entity* entity, struct touchscreen_area* area);
void SetLeaderAction(void);
bool ShouldLeaderKeepRunning(void);
void CheckLeaderTile(void);
void ChangeLeader(void);
void UseSingleUseItemWrapper(struct entity* user);
void UseSingleUseItem(struct entity* user, struct entity* target);
void UseThrowableItem(struct entity* user);
void ResetDamageData(struct damage_data* damage);
void FreeLoadedAttackSpriteAndMore(void);
uint16_t SetAndLoadCurrentAttackAnimation(enum pack_file_id pack_id, uint16_t file_index);
void ClearLoadedAttackSprite(void);
uint16_t GetLoadedAttackSpriteId(void);
int DungeonGetTotalSpriteFileSize(enum monster_id monster_id);
uint16_t DungeonGetSpriteIndex(enum monster_id monster_id);
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
void HandleFaint(struct entity* fainted_entity, union damage_source damage_source,
                 struct entity* killer);
void MoveMonsterToPos(struct entity* entity, int x_pos, int y_pos, undefined param_4);
void CreateMonsterSummaryFromMonster(struct monster_summary* monster_summary,
                                     struct monster* monster);
void UpdateAiTargetPos(struct entity* monster);
void SetMonsterTypeAndAbility(struct entity* target);
void TryActivateSlowStart(void);
void TryActivateArtificialWeatherAbilities(void);
int GetMonsterApparentId(struct entity* target, enum monster_id current_id);
void TryActivateTraceAndColorChange(struct entity* attacker, struct entity* defender,
                                    struct move* move);
bool DefenderAbilityIsActive(struct entity* attacker, struct entity* defender,
                             enum ability_id ability_id, bool attacker_ability_enabled);
bool IsMonster(struct entity* entity);
void TryActivateConversion2(struct entity* attacker, struct entity* defender, struct move* move);
void TryActivateTruant(struct entity* entity);
void TryPointCameraToMonster(struct entity* entity, undefined param_2, undefined param_3);
void RestorePpAllMovesSetFlags(struct entity* entity);
bool CheckTeamMemberIdxVeneer(int member_idx);
bool IsMonsterIdInNormalRangeVeneer(enum monster_id monster_id);
void BoostIQ(struct entity* entity, int iq_boost, bool suppress_logs);
bool ShouldMonsterHeadToStairs(struct entity* entity);
bool MewSpawnCheck(enum monster_id monster_id, bool fail_if_mew);
void TryEndStatusWithAbility(struct entity* attacker, struct entity* defender);
bool ExclusiveItemEffectIsActive(struct entity* entity, enum exclusive_item_effect_id effect_id);
struct entity* GetTeamMemberWithIqSkill(enum iq_skill_id iq_skill);
bool TeamMemberHasEnabledIqSkill(enum iq_skill_id iq_skill);
bool TeamLeaderIqSkillIsEnabled(enum iq_skill_id iq_skill);
int CountMovesOutOfPp(struct entity* entity);
bool HasSuperEffectiveMoveAgainstUser(struct entity* user, struct entity* target,
                                      bool ignore_moves_with_max_ginseng_not_99);
bool TryEatItem(struct entity* user, struct entity* target);
bool CheckSpawnThreshold(enum monster_id monster_id);
bool HasLowHealth(struct entity* entity);
bool AreEntitiesAdjacent(struct entity* first, struct entity* second);
bool IsSpecialStoryAlly(struct monster* monster);
bool IsExperienceLocked(struct monster* monster);
void InitOtherMonsterData(struct entity* entity, int fixed_room_stats_index, enum direction_id dir);
void InitEnemySpawnStats(void);
void InitEnemyStatsAndMoves(struct move* move_list, int16_t* hp, uint8_t* offensive_stats,
                            uint8_t* defensive_stats);
void SpawnTeam(undefined param_1);
void SpawnInitialMonsters(void);
struct entity* SpawnMonster(struct spawned_monster_data* monster_data, bool cannot_be_asleep);
void InitTeamMember(enum monster_id, int16_t x_position, int16_t y_position,
                    struct team_member* team_member_data, undefined param_5, undefined param_6,
                    undefined param_7, undefined param_8, undefined param_9);
void InitMonster(undefined param_1, struct entity* entity, struct spawned_monster_data* spawn_data,
                 undefined* param_4);
void SubInitMonster(struct monster* monster, bool flag);
void MarkShopkeeperSpawn(int x, int y, enum monster_id monster_id, enum monster_behavior behavior);
void SpawnShopkeepers(void);
uint16_t GetMaxHpAtLevel(enum monster_id monster_id, int level);
uint8_t GetOffensiveStatAtLevel(enum monster_id monster_id, int level, int stat_idx);
uint8_t GetDefensiveStatAtLevel(enum monster_id monster_id, int level, int stat_idx);
void GetOutlawSpawnData(struct spawned_target_data* outlaw);
void ExecuteMonsterAction(struct entity* monster);
void TryActivateFlashFireOnAllMonsters(void);
bool HasStatusThatPreventsActing(struct entity* monster);
enum mobility_type GetMobilityTypeCheckSlip(enum monster_id species, bool walk_on_water);
enum mobility_type GetMobilityTypeCheckSlipAndFloating(struct entity* monster,
                                                       enum monster_id species);
bool IsInvalidSpawnTile(enum monster_id monster_id, struct tile* tile);
enum mobility_type GetMobilityTypeAfterIqSkills(struct entity* monster,
                                                enum mobility_type mobility_type);
bool CannotStandOnTile(struct entity* monster, struct tile* tile);
int CalcSpeedStage(struct entity* entity, int counter_weight);
int CalcSpeedStageWrapper(struct entity* entity);
int GetNumberOfAttacks(struct entity* entity);
enum display_name_type GetMonsterDisplayNameType(struct entity* entity);
void GetMonsterName(char* buffer, struct monster* target_info);
bool IsMonsterDrowsy(struct entity* monster);
bool MonsterHasNonvolatileNonsleepStatus(struct entity* monster);
bool MonsterHasImmobilizingStatus(struct entity* monster);
bool MonsterHasAttackInterferingStatus(struct entity* monster);
bool MonsterHasSkillInterferingStatus(struct entity* monster);
bool MonsterHasLeechSeedStatus(struct entity* monster);
bool MonsterHasWhifferStatus(struct entity* monster);
bool IsMonsterVisuallyImpaired(struct entity* monster, bool check_held_item);
bool IsMonsterMuzzled(struct entity* monster);
bool MonsterHasMiracleEyeStatus(struct entity* monster);
bool MonsterHasNegativeStatus(struct entity* monster, bool check_held_item);
bool IsMonsterSleeping(struct entity* monster);
bool CanMonsterMoveInDirection(struct entity* monster, enum direction_id direction);
enum mobility_type GetDirectionalMobilityType(struct entity* monster,
                                              enum mobility_type base_mobility,
                                              enum direction_id direction);
bool IsMonsterCornered(struct entity* monster);
bool CanAttackInDirection(struct entity* monster, enum direction_id direction);
bool CanAiMonsterMoveInDirection(struct entity* monster, enum direction_id direction,
                                 bool* out_monster_in_target_position);
bool ShouldMonsterRunAway(struct entity* monster);
bool ShouldMonsterRunAwayVariation(struct entity* monster, undefined param_2);
bool SafeguardIsActive(struct entity* user, struct entity* target, bool log_message);
bool LeafGuardIsActive(struct entity* user, struct entity* target, bool log_message);
bool IsProtectedFromStatDrops(struct entity* user, struct entity* target, bool log_message);
bool NoGastroAcidStatus(struct entity* entity);
bool AbilityIsActive(struct entity* entity, enum ability_id ability_id);
bool AbilityIsActiveVeneer(struct entity* entity, enum ability_id ability_id);
bool OtherMonsterAbilityIsActive(struct entity* user, enum ability_id ability_id);
bool LevitateIsActive(struct entity* entity);
bool MonsterIsType(struct entity* entity, enum type_id type_id);
bool IsTypeAffectedByGravity(struct entity* entity, enum type_id type_id);
bool HasTypeAffectedByGravity(struct entity* entity, enum type_id type_id);
bool CanSeeInvisibleMonsters(struct entity* entity);
bool HasDropeyeStatus(struct entity* entity);
bool IqSkillIsEnabled(struct entity* entity, enum iq_skill_id iq_id);
void UpdateIqSkills(struct monster* monster);
enum type_id GetMoveTypeForMonster(struct entity* entity, struct move* move);
int GetMovePower(struct entity* entity, struct move* move);
bool UpdateStateFlags(struct monster* monster, uint16_t mask, bool set_flags);
bool IsProtectedFromNegativeStatus(struct entity* user, struct entity* target, bool log_message);
void AddExpSpecial(struct entity* attacker, struct entity* defender, int base_exp);
void EnemyEvolution(struct entity* entity);
void LevelUpItemEffect(struct entity* user, struct entity* target, int levels, bool message,
                       bool dialogue);
bool TryDecreaseLevel(struct entity* user, struct entity* target, int n_levels);
bool LevelUp(struct entity* user, struct entity* target, bool message, bool dialogue);
void GetMonsterMoves(struct move_id_16* out_moves, enum monster_id monster_id, int level);
void EvolveMonster(struct entity* user, struct entity* target, enum monster_id new_monster_id);
uint8_t GetSleepAnimationId(struct entity* entity);
bool DisplayActions(struct entity* param_1);
void CheckNonLeaderTile(struct entity* entity);
bool EndNegativeStatusCondition(struct entity* user, struct entity* target, bool animation,
                                bool fail_message, bool remove_wrapping);
bool EndNegativeStatusConditionWrapper(struct entity* user, struct entity* target, bool animation,
                                       bool fail_message);
void TransferNegativeStatusCondition(struct entity* user, struct entity* target);
void EndSleepClassStatus(struct entity* user, struct entity* target);
void EndBurnClassStatus(struct entity* user, struct entity* target);
void EndFrozenClassStatus(struct entity* user, struct entity* target, bool log);
void EndCringeClassStatus(struct entity* user, struct entity* target);
void EndReflectClassStatus(struct entity* user, struct entity* target);
void TryRemoveSnatchedMonsterFromDungeonStruct(struct entity* user, struct entity* target);
void EndCurseClassStatus(struct entity* user, struct entity* target,
                         uint8_t next_curse_class_status_being_applied, bool log_message);
void EndLeechSeedClassStatus(struct entity* user, struct entity* target);
void EndSureShotClassStatus(struct entity* user, struct entity* target);
void EndInvisibleClassStatus(struct entity* user, struct entity* target, bool no_slip_message);
void EndBlinkerClassStatus(struct entity* user, struct entity* target);
void EndMuzzledStatus(struct entity* user, struct entity* target);
void EndMiracleEyeStatus(struct entity* user, struct entity* target);
void EndMagnetRiseStatus(struct entity* user, struct entity* target);
bool TransferNegativeBlinkerClassStatus(struct entity* user, struct entity* target);
void EndFrozenStatus(struct entity* user, struct entity* target);
void EndProtectStatus(struct entity* user, struct entity* target);
void TryRestoreRoostTyping(struct entity* user, struct entity* target);
void TryTriggerMonsterHouse(struct entity* entity, bool outside_enemies);
bool ShouldMonsterFollowLeader(struct entity* monster);
void RunMonsterAi(struct entity* monster, undefined param_2);
void ApplyDamageAndEffects(struct entity* attacker, struct entity* defender,
                           struct damage_data* damage_data, bool false_swipe, bool exp_on_faint,
                           union damage_source damage_source, bool defender_response);
bool ApplyDamage(struct entity* attacker, struct entity* defender, struct damage_data* damage_data,
                 bool false_swipe, bool exp_on_faint, union damage_source damage_source);
bool AftermathCheck(struct entity* attacker, struct entity* defender,
                    union damage_source damage_source);
enum type_matchup GetTypeMatchupBothTypes(struct entity* attacker, struct entity* defender,
                                          enum type_id attack_type);
bool ScrappyShouldActivate(struct entity* attacker, struct entity* defender,
                           enum type_id attack_type);
bool IsTypeIneffectiveAgainstGhost(enum type_id attack_type);
bool GhostImmunityIsActive(struct entity* attacker, struct entity* defender, int target_type_idx);
enum type_matchup GetTypeMatchup(struct entity* attacker, struct entity* defender,
                                 int target_type_idx, enum type_id attack_type);
bool CalcTypeBasedDamageEffects(struct fx64* damage_mult_out, struct entity* attacker,
                                struct entity* defender, int attack_power, enum type_id attack_type,
                                struct damage_data* damage_out, bool partial);
void CalcDamage(struct entity* attacker, struct entity* defender, enum type_id attack_type,
                int attack_power, int crit_chance, struct damage_data* damage_out,
                int damage_mult_fp, enum move_id move_id, bool full_calc);
void ApplyDamageAndEffectsWrapper(struct entity* monster, int damage, enum damage_message message,
                                  union damage_source damage_source);
void CalcRecoilDamageFixed(struct entity* attacker, int fixed_damage, undefined4 param_3,
                           struct damage_data* damage_out, enum move_id move_id,
                           enum type_id attack_type, union damage_source damage_source,
                           enum damage_message damage_message, undefined4 param_9,
                           undefined4 param_10);
void CalcDamageFixed(struct entity* attacker, struct entity* defender, int fixed_damage,
                     bool exp_on_faint, struct damage_data* damage_out, enum type_id attack_type,
                     enum move_category move_category, union damage_source damage_source,
                     enum damage_message damage_message, undefined4 param_10, undefined4 param_11);
void CalcDamageFixedNoCategory(struct entity* attacker, struct entity* defender, int fixed_damage,
                               bool exp_on_faint, struct damage_data* damage_out,
                               enum type_id attack_type, union damage_source damage_source,
                               enum damage_message damage_message, undefined4 param_9,
                               undefined4 param_10);
void CalcDamageFixedWrapper(struct entity* attacker, struct entity* defender, int fixed_damage,
                            bool exp_on_faint, struct damage_data* damage_out,
                            enum type_id attack_type, enum move_category move_category,
                            union damage_source damage_source, enum damage_message damage_message,
                            undefined4 param_10, undefined4 param_11);
void UpdateShopkeeperModeAfterAttack(struct entity* attacker, struct entity* defender);
void UpdateShopkeeperModeAfterTrap(struct entity* shopkeeper, bool non_team_member);
void ResetDamageCalcDiagnostics(void);
bool SpecificRecruitCheck(enum monster_id monster_id);
bool RecruitCheck(struct entity* user, struct entity* target);
bool TryRecruit(struct entity* user, struct entity* recruit);
void TrySpawnMonsterAndTickSpawnCounter(void);
void TryNonLeaderItemPickUp(struct entity* entity);
bool AuraBowIsActive(struct entity* entity);
int ExclusiveItemOffenseBoost(struct entity* entity, int move_category_idx);
int ExclusiveItemDefenseBoost(struct entity* entity, int move_category_idx);
int TeamMemberHasItemActive(struct entity* monsters, enum item_id item_id);
bool TeamMemberHasExclusiveItemEffectActive(enum exclusive_item_effect_id effect_id);
void TrySpawnEnemyItemDrop(struct entity* attacker, struct entity* defender);
void TickNoSlipCap(struct entity* entity);
void TickStatusAndHealthRegen(struct entity* entity);
void InflictSleepStatusSingle(struct entity* entity, int turns);
void TryInflictSleepStatus(struct entity* user, struct entity* target, int turns, bool log_failure);
bool IsProtectedFromSleepClassStatus(struct entity* user, struct entity* target,
                                     bool ignore_safeguard, bool ingnore_protections,
                                     bool log_failure);
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
void TryInflictShadowHoldStatus(struct entity* user, struct entity* target, bool check_only);
void TryInflictIngrainStatus(struct entity* user, struct entity* target);
void TryInflictWrappedStatus(struct entity* user, struct entity* target);
void FreeOtherWrappedMonsters(struct entity* entity);
void TryInflictPetrifiedStatus(struct entity* user, struct entity* target);
void LowerOffensiveStat(struct entity* user, struct entity* target, int stat_idx, int16_t n_stages,
                        bool check_is_protected_from_stat_drops,
                        bool log_message_if_protected_from_stat_drops);
void LowerDefensiveStat(struct entity* user, struct entity* target, int stat_idx, int16_t n_stages,
                        bool check_is_protected_from_stat_drops,
                        bool log_message_if_protected_from_stat_drops);
void BoostOffensiveStat(struct entity* user, struct entity* target, int stat_idx, int16_t n_stages);
void BoostDefensiveStat(struct entity* user, struct entity* target, int stat_idx, int16_t n_stages);
int FlashFireShouldActivate(struct entity* attacker, struct entity* defender);
void ActivateFlashFire(struct entity* attacker, struct entity* defender);
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
bool ExclusiveItemEffectIsActiveWithLogging(struct entity* user, struct entity* target,
                                            bool should_log, int message_id,
                                            enum exclusive_item_effect_id effect_id);
bool TryActivateQuickFeet(struct entity* attacker, struct entity* defender);
void TryInflictTerrifiedStatus(struct entity* user, struct entity* target);
bool TryInflictGrudgeStatus(struct entity* user, struct entity* target, bool log_message);
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
void TryInflictDestinyBondStatus(struct entity* user, struct entity* target);
void TryInflictSureShotStatus(struct entity* user, struct entity* target);
void TryInflictWhifferStatus(struct entity* user, struct entity* target);
void TryInflictSetDamageStatus(struct entity* user, struct entity* target);
void TryInflictFocusEnergyStatus(struct entity* user, struct entity* target);
bool TryInflictDecoyStatus(struct entity* user, struct entity* target);
void TryInflictCurseStatus(struct entity* user, struct entity* target);
void TryInflictSnatchStatus(struct entity* user, struct entity* target);
bool TryInflictTauntStatus(struct entity* user, struct entity* target);
bool TryInflictStockpileStatus(struct entity* user, struct entity* target);
void TryInflictInvisibleStatus(struct entity* user, struct entity* target);
bool TryInflictPerishSongStatus(struct entity* user, struct entity* target, bool only_check);
bool TryInflictEncoreStatus(struct entity* user, struct entity* target, bool only_check);
void TryDecreaseBelly(struct entity* user, struct entity* target, int belly_lost,
                      int max_belly_shrink);
void TryIncreaseBelly(struct entity* user, struct entity* target, int belly_restoration,
                      int max_belly_boost, bool log_failure);
bool TryInflictMuzzledStatus(struct entity* user, struct entity* target, bool only_check);
void TryTransform(struct entity* user, struct entity* target);
void TryInflictMobileStatus(struct entity* user, struct entity* target);
bool TryInflictExposedStatus(struct entity* user, struct entity* target, int effect_id,
                             bool only_check);
void TryActivateIdentifyCondition(struct entity* user, struct entity* target);
bool TryInflictBlinkerStatus(struct entity* user, struct entity* target, bool check_only,
                             bool log_failure);
bool IsBlinded(struct entity* entity, bool check_held_item);
bool TryInflictCrossEyedStatus(struct entity* user, struct entity* target, bool check_only);
void TryInflictEyedropStatus(struct entity* user, struct entity* target);
bool TryInflictSlipStatus(struct entity* user, struct entity* target);
bool TryInflictDropeyeStatus(struct entity* user, struct entity* target);
void RestoreAllMovePP(struct entity* user, struct entity* target, int pp, bool suppress_logs);
void RestoreOneMovePP(struct entity* user, struct entity* target, int move_index, int pp,
                      bool log_message);
void RestoreRandomMovePP(struct entity* user, struct entity* target, int pp, bool log_message);
void ApplyProteinEffect(struct entity* user, struct entity* target, int stat_boost);
void ApplyCalciumEffect(struct entity* user, struct entity* target, int stat_boost);
void ApplyIronEffect(struct entity* user, struct entity* target, int stat_boost);
void ApplyZincEffect(struct entity* user, struct entity* target, int stat_boost);
void TryInflictLongTossStatus(struct entity* user, struct entity* target);
void TryInflictPierceStatus(struct entity* user, struct entity* target);
bool TryInflictGastroAcidStatus(struct entity* user, struct entity* target, bool log_message,
                                bool check_only);
void SetAquaRingHealingCountdownTo4(struct entity* entity);
void ApplyAquaRingHealing(struct entity* entity);
void TryInflictAquaRingStatus(struct entity* user, struct entity* target);
void TryInflictLuckyChantStatus(struct entity* user, struct entity* target);
bool TryInflictHealBlockStatus(struct entity* user, struct entity* target, bool log_message,
                               bool check_only);
bool MonsterHasEmbargoStatus(struct entity* entity);
void LogItemBlockedByEmbargo(struct entity* entity);
bool TryInflictEmbargoStatus(struct entity* user, struct entity* target, bool log_message,
                             bool check_only);
bool TryInflictMiracleEyeStatus(struct entity* user, struct entity* target, bool check_only);
void TryInflictMagnetRiseStatus(struct entity* user, struct entity* target);
bool IsFloating(struct entity* entity);
void TryInflictSafeguardStatus(struct entity* user, struct entity* target);
void TryInflictMistStatus(struct entity* user, struct entity* target);
void TryInflictWishStatus(struct entity* user, struct entity* target);
void TryInflictMagicCoatStatus(struct entity* user, struct entity* target);
void TryInflictLightScreenStatus(struct entity* user, struct entity* target);
void TryInflictReflectStatus(struct entity* user, struct entity* target);
void TryInflictProtectStatus(struct entity* user, struct entity* target);
void TryInflictMirrorCoatStatus(struct entity* user, struct entity* target);
void TryInflictEndureStatus(struct entity* user, struct entity* target);
void TryInflictMirrorMoveStatus(struct entity* user, struct entity* target);
void TryInflictConversion2Status(struct entity* user, struct entity* target);
void TryInflictVitalThrowStatus(struct entity* user, struct entity* target);
void TryResetStatChanges(struct entity* attacker, struct entity* defender, bool force_animation);
int MirrorMoveIsActive(struct entity* entity);
int MistIsActive(struct entity* entity);
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
bool ShouldUsePp(struct entity* entity);
struct move_target_and_range GetEntityMoveTargetAndRange(struct entity* entity, struct move* move,
                                                         bool is_ai);
struct natural_gift_item_info* GetEntityNaturalGiftInfo(struct entity* entity);
enum type_id GetEntityWeatherBallType(struct entity* entity);
void ActivateMotorDrive(struct entity* entity);
void TryActivateFrisk(struct entity* attacker, struct entity* defender);
void TryActivateBadDreams(struct entity* entity);
void ActivateStench(struct entity* entity);
void TryActivateSteadfast(struct entity* attacker, struct entity* defender);
bool IsInSpawnList(undefined* spawn_list, enum monster_id monster_id);
int ChangeShayminForme(struct entity* entity, int forme);
void ApplyItemEffect(undefined4 param_1, undefined4 param_2, undefined4 param_3,
                     struct entity* attacker, struct entity* defender, struct item* thrown_item);
void ApplyCheriBerryEffect(struct entity* user, struct entity* target);
void ApplyPechaBerryEffect(struct entity* user, struct entity* target);
void ApplyRawstBerryEffect(struct entity* user, struct entity* target);
void ApplyHungerSeedEffect(struct entity* user, struct entity* target);
void ApplyVileSeedEffect(struct entity* user, struct entity* target);
void ApplyViolentSeedEffect(struct entity* user, struct entity* target);
void ApplyGinsengEffect(struct entity* user, struct entity* target);
void ApplyBlastSeedEffect(struct entity* user, struct entity* target, bool thrown);
void ApplyGummiBoostsDungeonMode(struct entity* user, struct entity* target,
                                 enum type_id gummi_type, int random_stat_boost);
bool CanMonsterUseItem(struct entity* entity, struct item* item);
void ApplyGrimyFoodEffect(struct entity* user, struct entity* target);
void ApplyMixElixirEffect(struct entity* user, struct entity* target);
void ApplyDoughSeedEffect(struct entity* user, struct entity* target);
void ApplyViaSeedEffect(struct entity* user, struct entity* target);
void ApplyGravelyrockEffect(struct entity* user, struct entity* target);
void ApplyGonePebbleEffect(struct entity* user, struct entity* target);
void ApplyGracideaEffect(struct entity* user, struct entity* target);
bool ShouldTryEatItem(enum item_id item_id);
int GetMaxPpWrapper(struct move* move);
bool MoveIsNotPhysical(enum move_id move_id);
bool CategoryIsNotPhysical(enum move_category category_id);
void TryDrought(struct entity* user);
void TryPounce(struct entity* user, struct entity* target, enum direction_id direction);
void TryBlowAway(struct entity* user, struct entity* target, enum direction_id direction);
void TryExplosion(struct entity* user, struct entity* target, struct position* pos, int radius,
                  enum type_id attack_type, union damage_source damage_source);
void TryAftermathExplosion(struct entity* user, struct entity* target, struct position* pos,
                           int radius, enum type_id attack_type, union damage_source damage_source);
void TryWarp(struct entity* user, struct entity* target, enum warp_type warp_type,
             struct position* position);
void EnsureCanStandCurrentTile(struct entity* entity);
void TryActivateNondamagingDefenderAbility(struct entity* entity);
void TryActivateNondamagingDefenderExclusiveItem(struct entity* attacker, struct entity* defender);
int GetMoveRangeDistance(struct entity* user, struct move* move, bool check_two_turn_moves);
bool MoveHitCheck(struct entity* attacker, struct entity* defender, struct move* move,
                  bool use_second_accuracy, bool never_miss_self);
bool IsHyperBeamVariant(struct move* move);
bool IsChargingTwoTurnMove(struct entity* user, struct move* move);
bool HasMaxGinsengBoost99(struct move* move);
bool TwoTurnMoveForcedMiss(struct entity* target, struct move* move);
bool DungeonRandOutcomeUserTargetInteraction(struct entity* user, struct entity* target,
                                             int percentage);
bool DungeonRandOutcomeUserAction(struct entity* user, int percentage);
bool CanAiUseMove(struct entity* monster, int move_index, bool extra_checks);
bool CanMonsterUseMove(struct entity* monster, struct move* move, bool extra_checks);
void UpdateMovePp(struct entity* entity, bool can_consume_pp);
union damage_source GetDamageSourceWrapper(struct move* move, enum item_id item_id);
int LowerSshort(int x);
void PlayMoveAnimation(struct entity* user, struct entity* target, struct move* move,
                       struct position* position);
uint16_t GetMoveAnimationId(struct move* move, enum weather_id apparent_weather,
                            bool should_play_alternative_animation);
bool ShouldMovePlayAlternativeAnimation(struct entity* user, struct move* move);
void ExecuteMoveEffect(undefined4* param_1, struct entity* attacker, struct move* move,
                       undefined4 param_4, undefined4 param_5);
bool DoMoveDamageInlined(struct entity* attacker, struct entity* defender, struct move* move,
                         enum item_id item_id);
int DealDamage(struct entity* attacker, struct entity* defender, struct move* move,
               int damage_mult_fp, enum item_id item_id);
int DealDamageWithTypeAndPowerBoost(struct entity* attacker, struct entity* defender,
                                    struct move* move, int damage_mult_fp, enum item_id item_id,
                                    enum type_id attack_type, int16_t power_boost);
int DealDamageProjectile(struct entity* attacker, struct entity* defender, struct move* move,
                         int power, int damage_mult_fp, enum item_id item_id);
int DealDamageWithType(struct entity* attacker, struct entity* defender, enum type_id attack_type,
                       struct move* move, int damage_mult_fp, enum item_id item_id);
int PerformDamageSequence(struct entity* attacker, struct entity* defender, struct move* move,
                          struct damage_data* damage_out, union damage_source damage_source);
bool StatusCheckerCheck(struct entity* attacker, struct move* move);
enum weather_id GetApparentWeather(struct entity* entity);
void TryWeatherFormChange(struct entity* entity);
void ActivateSportCondition(bool water_sport);
bool TryActivateWeather(bool param_1, bool param_2);
int DigitCount(int n);
void LoadTextureUi(void);
int DisplayNumberTextureUi(int16_t x, int16_t y, int n, int ally_mode);
int DisplayCharTextureUi(struct render_3d_element_64* element64, int16_t x, int16_t y, int char_id,
                         int16_t param_5);
void DisplayUi(void);
struct tile* GetTile(int x, int y);
struct tile* GetTileSafe(int x, int y);
bool IsFullFloorFixedRoom(void);
bool IsCurrentTilesetBackground(void);
void TrySpawnGoldenChamber(void);
void CountItemsOnFloorForAcuteSniffer(void);
void GetStairsSpawnPosition(int16_t* x, int16_t* y);
bool PositionIsOnStairs(int x, int y);
uint8_t GetStairsRoom(void);
uint16_t GetDefaultTileTextureId(void);
void DetermineAllTilesWalkableNeighbors(void);
void DetermineTileWalkableNeighbors(int x, int y);
void UpdateTrapsVisibility(void);
void DrawTileGrid(struct position* pos, undefined param_2, undefined param_3, undefined param_4);
void HideTileGrid(void);
void DiscoverMinimap(struct position* pos);
bool PositionHasItem(struct position* pos);
bool PositionHasMonster(struct position* pos);
bool TrySmashWall(struct position* pos);
bool IsWaterTileset(void);
enum monster_id GetRandomSpawnMonsterID(void);
bool NearbyAllyIqSkillIsEnabled(struct entity* entity, enum iq_skill_id iq_skill);
void ResetGravity(void);
bool GravityIsActive(void);
bool TryActivateGravity(void);
bool ShouldBoostKecleonShopSpawnChance(void);
void SetShouldBoostKecleonShopSpawnChance(bool value);
void UpdateShouldBoostKecleonShopSpawnChance(void);
void SetDoughSeedFlag(bool value);
void TrySpawnDoughSeedPoke(void);
bool IsSecretBazaar(void);
bool ShouldBoostHiddenStairsSpawnChance(void);
void SetShouldBoostHiddenStairsSpawnChance(bool value);
void UpdateShouldBoostHiddenStairsSpawnChance(void);
bool IsSecretRoom(void);
bool IsSecretFloor(void);
bool HiddenStairsPresent(void);
void HiddenStairsTrigger(bool show_message);
undefined4 GetDungeonGenInfoUnk0C(void);
struct minimap_display_data* GetMinimapData(void);
void DrawMinimapTile(int x, int y);
void UpdateMinimap(void);
void SetMinimapDataE447(uint8_t value);
uint8_t GetMinimapDataE447(void);
void SetMinimapDataE448(uint8_t value);
void InitWeirdMinimapMatrix(void);
void InitMinimapDisplayTile(struct minimap_display_tile* minimap_display_tile, undefined* ptr);
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
void MarkNonEnemySpawns(struct floor_properties* floor_props, bool empty_monster_house);
void MarkEnemySpawns(struct floor_properties* floor_props, bool empty_monster_house);
void SetSecondaryTerrainOnWall(struct tile* tile);
void GenerateSecondaryTerrainFormations(uint8_t test_flag, struct floor_properties* floor_props);
bool StairsAlwaysReachable(int x_stairs, int y_stairs, bool mark_unreachable);
union fixed_room_action GetNextFixedRoomAction(void);
void ConvertWallsToChasms(void);
void ResetInnerBoundaryTileRows(void);
void ResetImportantSpawnPositions(struct dungeon_generation_info* gen_info);
void SpawnStairs(uint8_t* pos, struct dungeon_generation_info* gen_info,
                 enum hidden_stairs_type hidden_stairs_type);
enum hidden_stairs_type GetHiddenStairsType(struct dungeon_generation_info* gen_info,
                                            struct floor_properties* floor_props);
int GetFinalKecleonShopSpawnChance(int base_kecleon_shop_chance);
void ResetHiddenStairsSpawn(void);
void PlaceFixedRoomTile(struct tile* tile, union fixed_room_action action, int x, int y);
enum direction_id FixedRoomActionParamToDirection(uint8_t fixed_room_action_param);
void ApplyKeyEffect(struct entity* user, struct entity* target);
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
bool IsSecretBazaarVeneer(void);
void GenerateStandardItem(struct item* item, enum item_id item_id,
                          enum gen_item_stickiness sticky_type);
void GenerateCleanItem(struct item* item, enum item_id item_id);
void TryLeaderItemPickUp(struct position* position, bool flag);
bool SpawnItem(struct position* position, struct item* item, bool flag);
void RemoveGroundItem(struct position* position, bool update_shop_counters);
void SpawnDroppedItemWrapper(struct entity* entity, struct position* pos, struct item* item,
                             undefined4 param_4);
void SpawnDroppedItem(struct entity* entity, struct entity* item_entity, struct item* item,
                      int param_4, int16_t* dir_xy, undefined param_6);
bool TryGenerateUnownStoneDrop(struct item* item, enum monster_id monster_id);
bool HasHeldItem(struct entity* entity, enum item_id item_id);
void GenerateMoneyQuantity(struct item* item, int max_amount);
bool CheckTeamItemsFlags(int flags);
void AddHeldItemToBag(struct monster* monster);
void RemoveEmptyItemsInBagWrapper(void);
void GenerateItem(struct item* item, enum item_id item_id, uint16_t quantity,
                  enum gen_item_stickiness sticky_type);
bool DoesProjectileHitTarget(struct entity* user, struct entity* target);
bool CheckActiveChallengeRequest(void);
struct mission_destination_info* GetMissionDestination(void);
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
void LogMessageByIdWithPopupCheckParticipants(struct entity* user, struct entity* target,
                                              int message_id, int idx, int16_t val);
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
void InitPortraitDungeon(struct portrait_box* portrait, enum monster_id monster_id,
                         enum portrait_emotion emotion);
void OpenMessageLog(undefined4 param_1, undefined4 param_2);
bool RunDungeonMode(undefined4* param_1, undefined4 param_2);
void DisplayDungeonTip(struct message_tip* message_tip, bool log);
void SetBothScreensWindowColorToDefault(void);
int GetPersonalityIndex(struct monster* monster);
void DisplayMessage(struct portrait_box* portrait, int message_id, bool wait_for_input);
void DisplayMessage2(struct portrait_box* portrait, int message_id, bool wait_for_input);
bool YesNoMenu(undefined param_1, int message_id, int default_option, undefined param_4);
void DisplayMessageInternal(int message_id, bool wait_for_input, struct portrait_box* portrait,
                            undefined4 param_4, undefined4 param_5, undefined4 param_6);
void OpenMenu(undefined param_1, undefined param_2, bool open_bag);
int OthersMenuLoop(void);
undefined OthersMenu(void);

#endif
