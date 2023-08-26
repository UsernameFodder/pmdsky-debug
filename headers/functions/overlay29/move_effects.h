#ifndef HEADERS_FUNCTIONS_OVERLAY29_MOVE_EFFECTS_H_
#define HEADERS_FUNCTIONS_OVERLAY29_MOVE_EFFECTS_H_

bool DoMoveDamage(struct entity* attacker, struct entity* defender, struct move* move,
                  enum item_id item_id);
bool DoMoveIronTail(struct entity* attacker, struct entity* defender, struct move* move,
                    enum item_id item_id);
bool DoMoveDamageMultihitUntilMiss(struct entity* attacker, struct entity* defender,
                                   struct move* move, enum item_id item_id);
bool DoMoveYawn(struct entity* attacker, struct entity* defender, struct move* move,
                enum item_id item_id);
bool DoMoveSleep(struct entity* attacker, struct entity* defender, struct move* move,
                 enum item_id item_id);
bool DoMoveNightmare(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveMorningSun(struct entity* attacker, struct entity* defender, struct move* move,
                      enum item_id item_id);
bool DoMoveVitalThrow(struct entity* attacker, struct entity* defender, struct move* move,
                      enum item_id item_id);
bool DoMoveDig(struct entity* attacker, struct entity* defender, struct move* move,
               enum item_id item_id);
bool DoMoveSweetScent(struct entity* attacker, struct entity* defender, struct move* move,
                      enum item_id item_id);
bool DoMoveCharm(struct entity* attacker, struct entity* defender, struct move* move,
                 enum item_id item_id);
bool DoMoveRainDance(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveHail(struct entity* attacker, struct entity* defender, struct move* move,
                enum item_id item_id);
bool DoMoveHealStatus(struct entity* attacker, struct entity* defender, struct move* move,
                      enum item_id item_id);
bool DoMoveBubble(struct entity* attacker, struct entity* defender, struct move* move,
                  enum item_id item_id);
bool DoMoveEncore(struct entity* attacker, struct entity* defender, struct move* move,
                  enum item_id item_id);
bool DoMoveRage(struct entity* attacker, struct entity* defender, struct move* move,
                enum item_id item_id);
bool DoMoveSuperFang(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMovePainSplit(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveTorment(struct entity* attacker, struct entity* defender, struct move* move,
                   enum item_id item_id);
bool DoMoveStringShot(struct entity* attacker, struct entity* defender, struct move* move,
                      enum item_id item_id);
bool DoMoveSwagger(struct entity* attacker, struct entity* defender, struct move* move,
                   enum item_id item_id);
bool DoMoveSnore(struct entity* attacker, struct entity* defender, struct move* move,
                 enum item_id item_id);
bool DoMoveScreech(struct entity* attacker, struct entity* defender, struct move* move,
                   enum item_id item_id);
bool DoMoveDamageCringe30(struct entity* attacker, struct entity* defender, struct move* move,
                          enum item_id item_id);
bool DoMoveWeatherBall(struct entity* attacker, struct entity* defender, struct move* move,
                       enum item_id item_id);
bool DoMoveWhirlpool(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveFakeTears(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveSpite(struct entity* attacker, struct entity* defender, struct move* move,
                 enum item_id item_id);
bool DoMoveFocusEnergy(struct entity* attacker, struct entity* defender, struct move* move,
                       enum item_id item_id);
bool DoMoveSmokescreen(struct entity* attacker, struct entity* defender, struct move* move,
                       enum item_id item_id);
bool DoMoveMirrorMove(struct entity* attacker, struct entity* defender, struct move* move,
                      enum item_id item_id);
bool DoMoveOverheat(struct entity* attacker, struct entity* defender, struct move* move,
                    enum item_id item_id);
bool DoMoveAuroraBeam(struct entity* attacker, struct entity* defender, struct move* move,
                      enum item_id item_id);
bool DoMoveMemento(struct entity* attacker, struct entity* defender, struct move* move,
                   enum item_id item_id);
bool DoMoveOctazooka(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveFlatter(struct entity* attacker, struct entity* defender, struct move* move,
                   enum item_id item_id);
bool DoMoveWillOWisp(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveReturn(struct entity* attacker, struct entity* defender, struct move* move,
                  enum item_id item_id);
bool DoMoveGrudge(struct entity* attacker, struct entity* defender, struct move* move,
                  enum item_id item_id);
bool DoMoveCounter(struct entity* attacker, struct entity* defender, struct move* move,
                   enum item_id item_id);
bool DoMoveDamageBurn10FlameWheel(struct entity* attacker, struct entity* defender,
                                  struct move* move, enum item_id item_id);
bool DoMoveDamageBurn10(struct entity* attacker, struct entity* defender, struct move* move,
                        enum item_id item_id);
bool DoMoveExpose(struct entity* attacker, struct entity* defender, struct move* move,
                  enum item_id item_id);
bool DoMoveDoubleTeam(struct entity* attacker, struct entity* defender, struct move* move,
                      enum item_id item_id);
bool DoMoveGust(struct entity* attacker, struct entity* defender, struct move* move,
                enum item_id item_id);
bool DoMoveBoostDefense1(struct entity* attacker, struct entity* defender, struct move* move,
                         enum item_id item_id);
bool DoMoveParalyze(struct entity* attacker, struct entity* defender, struct move* move,
                    enum item_id item_id);
bool DoMoveBoostAttack1(struct entity* attacker, struct entity* defender, struct move* move,
                        enum item_id item_id);
bool DoMoveRazorWind(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveBide(struct entity* attacker, struct entity* defender, struct move* move,
                enum item_id item_id);
bool DoMoveBideUnleash(struct entity* attacker, struct entity* defender, struct move* move,
                       enum item_id item_id);
bool DoMoveCrunch(struct entity* attacker, struct entity* defender, struct move* move,
                  enum item_id item_id);
bool DoMoveDamageCringe20(struct entity* attacker, struct entity* defender, struct move* move,
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
bool DoMoveDamageLowerSpeed100(struct entity* attacker, struct entity* defender, struct move* move,
                               enum item_id item_id);
bool DoMoveFocusPunch(struct entity* attacker, struct entity* defender, struct move* move,
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
bool DoMoveShadowHold(struct entity* attacker, struct entity* defender, struct move* move,
                      enum item_id item_id);
bool DoMoveHaze(struct entity* attacker, struct entity* defender, struct move* move,
                enum item_id item_id);
bool DoMoveDamageMultihitFatigue(struct entity* attacker, struct entity* defender,
                                 struct move* move, enum item_id item_id);
bool DoMoveDamageWeightDependent(struct entity* attacker, struct entity* defender,
                                 struct move* move, enum item_id item_id);
bool DoMoveDamageBoostAllStats(struct entity* attacker, struct entity* defender, struct move* move,
                               enum item_id item_id);
bool DoMoveSynthesis(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveBoostSpeed1(struct entity* attacker, struct entity* defender, struct move* move,
                       enum item_id item_id);
bool DoMoveRapidSpin(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveSureShot(struct entity* attacker, struct entity* defender, struct move* move,
                    enum item_id item_id);
bool DoMoveCosmicPower(struct entity* attacker, struct entity* defender, struct move* move,
                       enum item_id item_id);
bool DoMoveSkyAttack(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveDamageFreeze15(struct entity* attacker, struct entity* defender, struct move* move,
                          enum item_id item_id);
bool DoMoveMeteorMash(struct entity* attacker, struct entity* defender, struct move* move,
                      enum item_id item_id);
bool DoMoveEndure(struct entity* attacker, struct entity* defender, struct move* move,
                  enum item_id item_id);
bool DoMoveLowerSpeed1(struct entity* attacker, struct entity* defender, struct move* move,
                       enum item_id item_id);
bool DoMoveDamageConfuse10(struct entity* attacker, struct entity* defender, struct move* move,
                           enum item_id item_id);
bool DoMovePsywave(struct entity* attacker, struct entity* defender, struct move* move,
                   enum item_id item_id);
bool DoMoveDamageLowerDefensiveStatVariable(struct entity* attacker, struct entity* defender,
                                            struct move* move, int stat_idx, int n_stages,
                                            int lower_chance, enum item_id item_id);
bool DoMovePsychoBoost(struct entity* attacker, struct entity* defender, struct move* move,
                       enum item_id item_id);
bool DoMoveUproar(struct entity* attacker, struct entity* defender, struct move* move,
                  enum item_id item_id);
bool DoMoveWaterSpout(struct entity* attacker, struct entity* defender, struct move* move,
                      enum item_id item_id);
bool DoMovePsychUp(struct entity* attacker, struct entity* defender, struct move* move,
                   enum item_id item_id);
bool DoMoveDamageWithRecoil(struct entity* attacker, struct entity* defender, struct move* move,
                            enum item_id item_id);
bool EntityIsValidMoveEffects(struct entity* entity);
bool DoMoveRecoverHp(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveEarthquake(struct entity* attacker, struct entity* defender, struct move* move,
                      enum item_id item_id);
enum nature_power_variant GetNaturePowerVariant(void);
bool DoMoveNaturePower(struct entity* attacker, struct entity* defender, struct move* move,
                       enum item_id item_id);
bool DoMoveDamageParalyze10(struct entity* attacker, struct entity* defender, struct move* move,
                            enum item_id item_id);
bool DoMoveSelfdestruct(struct entity* attacker, struct entity* defender, struct move* move,
                        enum item_id item_id);
bool DoMoveShadowBall(struct entity* attacker, struct entity* defender, struct move* move,
                      enum item_id item_id);
bool DoMoveCharge(struct entity* attacker, struct entity* defender, struct move* move,
                  enum item_id item_id);
bool DoMoveThunderbolt(struct entity* attacker, struct entity* defender, struct move* move,
                       enum item_id item_id);
bool DoMoveMist(struct entity* attacker, struct entity* defender, struct move* move,
                enum item_id item_id);
bool DoMoveFissure(struct entity* attacker, struct entity* defender, struct move* move,
                   enum item_id item_id);
bool DoMoveDamageCringe10(struct entity* attacker, struct entity* defender, struct move* move,
                          enum item_id item_id);
bool DoMoveSafeguard(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveAbsorb(struct entity* attacker, struct entity* defender, struct move* move,
                  enum item_id item_id);
bool DefenderAbilityIsActiveMoveEffects(struct entity* attacker, struct entity* defender,
                                        enum ability_id ability_id, bool attacker_ability_enabled);
bool DoMoveSkillSwap(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveSketch(struct entity* attacker, struct entity* defender, struct move* move,
                  enum item_id item_id);
bool DoMoveHeadbutt(struct entity* attacker, struct entity* defender, struct move* move,
                    enum item_id item_id);
bool DoMoveDoubleEdge(struct entity* attacker, struct entity* defender, struct move* move,
                      enum item_id item_id);
bool DoMoveSandstorm(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveLowerAccuracy1(struct entity* attacker, struct entity* defender, struct move* move,
                          enum item_id item_id);
bool DoMoveDamagePoison40(struct entity* attacker, struct entity* defender, struct move* move,
                          enum item_id item_id);
bool DoMoveGrowth(struct entity* attacker, struct entity* defender, struct move* move,
                  enum item_id item_id);
bool DoMoveSacredFire(struct entity* attacker, struct entity* defender, struct move* move,
                      enum item_id item_id);
bool DoMoveOhko(struct entity* attacker, struct entity* defender, struct move* move,
                enum item_id item_id);
bool DoMoveSolarBeam(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveSonicBoom(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveFly(struct entity* attacker, struct entity* defender, struct move* move,
               enum item_id item_id);
bool DoMoveExplosion(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveDive(struct entity* attacker, struct entity* defender, struct move* move,
                enum item_id item_id);
bool DoMoveWaterfall(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveDamageLowerAccuracy40(struct entity* attacker, struct entity* defender,
                                 struct move* move, enum item_id item_id);
bool DoMoveStockpile(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveTwister(struct entity* attacker, struct entity* defender, struct move* move,
                   enum item_id item_id);
bool DoMoveTwineedle(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveRecoverHpTeam(struct entity* attacker, struct entity* defender, struct move* move,
                         enum item_id item_id);
bool DoMoveMinimize(struct entity* attacker, struct entity* defender, struct move* move,
                    enum item_id item_id);
bool DoMoveSeismicToss(struct entity* attacker, struct entity* defender, struct move* move,
                       enum item_id item_id);
bool DoMoveConfuse(struct entity* attacker, struct entity* defender, struct move* move,
                   enum item_id item_id);
bool DoMoveTaunt(struct entity* attacker, struct entity* defender, struct move* move,
                 enum item_id item_id);
bool DoMoveMoonlight(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveHornDrill(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveSwordsDance(struct entity* attacker, struct entity* defender, struct move* move,
                       enum item_id item_id);
bool DoMoveConversion(struct entity* attacker, struct entity* defender, struct move* move,
                      enum item_id item_id);
bool DoMoveConversion2(struct entity* attacker, struct entity* defender, struct move* move,
                       enum item_id item_id);
bool DoMoveHelpingHand(struct entity* attacker, struct entity* defender, struct move* move,
                       enum item_id item_id);
bool DoMoveBoostDefense2(struct entity* attacker, struct entity* defender, struct move* move,
                         enum item_id item_id);
bool DoMoveWarp(struct entity* attacker, struct entity* defender, struct move* move,
                enum item_id item_id);
bool DoMoveThundershock(struct entity* attacker, struct entity* defender, struct move* move,
                        enum item_id item_id);
bool DoMoveThunderWave(struct entity* attacker, struct entity* defender, struct move* move,
                       enum item_id item_id);
bool DoMoveZapCannon(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveBlock(struct entity* attacker, struct entity* defender, struct move* move,
                 enum item_id item_id);
bool DoMovePoison(struct entity* attacker, struct entity* defender, struct move* move,
                  enum item_id item_id);
bool DoMoveToxic(struct entity* attacker, struct entity* defender, struct move* move,
                 enum item_id item_id);
bool DoMovePoisonFang(struct entity* attacker, struct entity* defender, struct move* move,
                      enum item_id item_id);
bool DoMoveDamagePoison18(struct entity* attacker, struct entity* defender, struct move* move,
                          enum item_id item_id);
bool DoMoveJumpKick(struct entity* attacker, struct entity* defender, struct move* move,
                    enum item_id item_id);
bool DoMoveBounce(struct entity* attacker, struct entity* defender, struct move* move,
                  enum item_id item_id);
bool DoMoveHiJumpKick(struct entity* attacker, struct entity* defender, struct move* move,
                      enum item_id item_id);
bool DoMoveTriAttack(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveSwapItems(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveTripleKick(struct entity* attacker, struct entity* defender, struct move* move,
                      enum item_id item_id);
bool DoMoveSport(struct entity* attacker, struct entity* defender, struct move* move,
                 enum item_id item_id);
bool DoMoveMudSlap(struct entity* attacker, struct entity* defender, struct move* move,
                   enum item_id item_id);
bool DoMoveDamageStealItem(struct entity* attacker, struct entity* defender, struct move* move,
                           enum item_id item_id);
bool DoMoveAmnesia(struct entity* attacker, struct entity* defender, struct move* move,
                   enum item_id item_id);
bool DoMoveNightShade(struct entity* attacker, struct entity* defender, struct move* move,
                      enum item_id item_id);
bool DoMoveGrowl(struct entity* attacker, struct entity* defender, struct move* move,
                 enum item_id item_id);
bool DoMoveSurf(struct entity* attacker, struct entity* defender, struct move* move,
                enum item_id item_id);
bool DoMoveRolePlay(struct entity* attacker, struct entity* defender, struct move* move,
                    enum item_id item_id);
bool DoMoveSunnyDay(struct entity* attacker, struct entity* defender, struct move* move,
                    enum item_id item_id);
bool DoMoveLowerDefense1(struct entity* attacker, struct entity* defender, struct move* move,
                         enum item_id item_id);
bool DoMoveWish(struct entity* attacker, struct entity* defender, struct move* move,
                enum item_id item_id);
bool DoMoveFakeOut(struct entity* attacker, struct entity* defender, struct move* move,
                   enum item_id item_id);
bool DoMoveSleepTalk(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMovePayDay(struct entity* attacker, struct entity* defender, struct move* move,
                  enum item_id item_id);
bool DoMoveAssist(struct entity* attacker, struct entity* defender, struct move* move,
                  enum item_id item_id);
bool DoMoveRest(struct entity* attacker, struct entity* defender, struct move* move,
                enum item_id item_id);
bool DoMoveIngrain(struct entity* attacker, struct entity* defender, struct move* move,
                   enum item_id item_id);
bool DoMoveSwallow(struct entity* attacker, struct entity* defender, struct move* move,
                   enum item_id item_id);
bool DoMoveCurse(struct entity* attacker, struct entity* defender, struct move* move,
                 enum item_id item_id);
bool DoMoveSuperpower(struct entity* attacker, struct entity* defender, struct move* move,
                      enum item_id item_id);
bool DoMoveSteelWing(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveSpitUp(struct entity* attacker, struct entity* defender, struct move* move,
                  enum item_id item_id);
bool DoMoveDynamicPunch(struct entity* attacker, struct entity* defender, struct move* move,
                        enum item_id item_id);
bool DoMoveKnockOff(struct entity* attacker, struct entity* defender, struct move* move,
                    enum item_id item_id);
bool DoMoveSplash(struct entity* attacker, struct entity* defender, struct move* move,
                  enum item_id item_id);
bool DoMoveSetDamage(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveBellyDrum(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveLightScreen(struct entity* attacker, struct entity* defender, struct move* move,
                       enum item_id item_id);
bool DoMoveSecretPower(struct entity* attacker, struct entity* defender, struct move* move,
                       enum item_id item_id);
bool DoMoveDamageConfuse30(struct entity* attacker, struct entity* defender, struct move* move,
                           enum item_id item_id);
bool DoMoveBulkUp(struct entity* attacker, struct entity* defender, struct move* move,
                  enum item_id item_id);
bool DoMovePause(struct entity* attacker, struct entity* defender, struct move* move,
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
bool DoMoveTransform(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMovePoisonTail(struct entity* attacker, struct entity* defender, struct move* move,
                      enum item_id item_id);
bool DoMoveBlowback(struct entity* attacker, struct entity* defender, struct move* move,
                    enum item_id item_id);
bool DoMoveCamouflage(struct entity* attacker, struct entity* defender, struct move* move,
                      enum item_id item_id);
bool DoMoveTailGlow(struct entity* attacker, struct entity* defender, struct move* move,
                    enum item_id item_id);
bool DoMoveDamageConstrict10(struct entity* attacker, struct entity* defender, struct move* move,
                             enum item_id item_id);
bool DoMovePerishSong(struct entity* attacker, struct entity* defender, struct move* move,
                      enum item_id item_id);
bool DoMoveWrap(struct entity* attacker, struct entity* defender, struct move* move,
                enum item_id item_id);
bool DoMoveSpikes(struct entity* attacker, struct entity* defender, struct move* move,
                  enum item_id item_id);
bool DoMoveMagnitude(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveMagicCoat(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveProtect(struct entity* attacker, struct entity* defender, struct move* move,
                   enum item_id item_id);
bool DoMoveDefenseCurl(struct entity* attacker, struct entity* defender, struct move* move,
                       enum item_id item_id);
bool DoMoveDecoy(struct entity* attacker, struct entity* defender, struct move* move,
                 enum item_id item_id);
bool DoMoveMistBall(struct entity* attacker, struct entity* defender, struct move* move,
                    enum item_id item_id);
bool DoMoveDestinyBond(struct entity* attacker, struct entity* defender, struct move* move,
                       enum item_id item_id);
bool DoMoveMirrorCoat(struct entity* attacker, struct entity* defender, struct move* move,
                      enum item_id item_id);
bool DoMoveCalmMind(struct entity* attacker, struct entity* defender, struct move* move,
                    enum item_id item_id);
bool DoMoveHiddenPower(struct entity* attacker, struct entity* defender, struct move* move,
                       enum item_id item_id);
bool DoMoveMetalClaw(struct entity* attacker, struct entity* defender, struct move* move,
                     int stat_idx, enum item_id item_id);
bool DoMoveAttract(struct entity* attacker, struct entity* defender, struct move* move,
                   enum item_id item_id);
bool DoMoveCopycat(struct entity* attacker, struct entity* defender, struct move* move,
                   enum item_id item_id);
bool DoMoveFrustration(struct entity* attacker, struct entity* defender, struct move* move,
                       enum item_id item_id);
bool DoMoveLeechSeed(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveMetronome(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveDreamEater(struct entity* attacker, struct entity* defender, struct move* move,
                      enum item_id item_id);
bool DoMoveSnatch(struct entity* attacker, struct entity* defender, struct move* move,
                  enum item_id item_id);
bool DoMoveRecycle(struct entity* attacker, struct entity* defender, struct move* move,
                   enum item_id item_id);
bool DoMoveReflect(struct entity* attacker, struct entity* defender, struct move* move,
                   enum item_id item_id);
bool DoMoveDragonRage(struct entity* attacker, struct entity* defender, struct move* move,
                      enum item_id item_id);
bool DoMoveDragonDance(struct entity* attacker, struct entity* defender, struct move* move,
                       enum item_id item_id);
bool DoMoveSkullBash(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveDamageLowerSpecialDefense50(struct entity* attacker, struct entity* defender,
                                       struct move* move, enum item_id item_id);
bool DoMoveStruggle(struct entity* attacker, struct entity* defender, struct move* move,
                    enum item_id item_id);
bool DoMoveRockSmash(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveSeeTrap(struct entity* attacker, struct entity* defender, struct move* move,
                   enum item_id item_id);
bool DoMoveTakeaway(struct entity* attacker, struct entity* defender, struct move* move,
                    enum item_id item_id);
bool DoMoveRebound(struct entity* attacker, struct entity* defender, struct move* move,
                   enum item_id item_id);
bool DoMoveSwitchPositions(struct entity* attacker, struct entity* defender, struct move* move,
                           enum item_id item_id);
bool DoMoveStayAway(struct entity* attacker, struct entity* defender, struct move* move,
                    enum item_id item_id);
bool DoMoveCleanse(struct entity* attacker, struct entity* defender, struct move* move,
                   enum item_id item_id);
bool DoMoveSiesta(struct entity* attacker, struct entity* defender, struct move* move,
                  enum item_id item_id);
bool DoMoveTwoEdge(struct entity* attacker, struct entity* defender, struct move* move,
                   enum item_id item_id);
bool DoMoveNoMove(struct entity* attacker, struct entity* defender, struct move* move,
                  enum item_id item_id);
bool DoMoveScan(struct entity* attacker, struct entity* defender, struct move* move,
                enum item_id item_id);
bool DoMovePowerEars(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveTransfer(struct entity* attacker, struct entity* defender, struct move* move,
                    enum item_id item_id);
bool DoMoveSlowDown(struct entity* attacker, struct entity* defender, struct move* move,
                    enum item_id item_id);
bool DoMoveSearchlight(struct entity* attacker, struct entity* defender, struct move* move,
                       enum item_id item_id);
bool DoMovePetrify(struct entity* attacker, struct entity* defender, struct move* move,
                   enum item_id item_id);
bool DoMovePounce(struct entity* attacker, struct entity* defender, struct move* move,
                  enum item_id item_id);
bool DoMoveTrawl(struct entity* attacker, struct entity* defender, struct move* move,
                 enum item_id item_id);
bool DoMoveEscape(struct entity* attacker, struct entity* defender, struct move* move,
                  enum item_id item_id);
bool DoMoveDrought(struct entity* attacker, struct entity* defender, struct move* move,
                   enum item_id item_id);
bool DoMoveTrapBuster(struct entity* attacker, struct entity* defender, struct move* move,
                      enum item_id item_id);
bool DoMoveWildCall(struct entity* attacker, struct entity* defender, struct move* move,
                    enum item_id item_id);
bool DoMoveInvisify(struct entity* attacker, struct entity* defender, struct move* move,
                    enum item_id item_id);
bool DoMoveOneShot(struct entity* attacker, struct entity* defender, struct move* move,
                   enum item_id item_id);
bool DoMoveHpGauge(struct entity* attacker, struct entity* defender, struct move* move,
                   enum item_id item_id);
bool DoMoveVacuumCut(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveReviver(struct entity* attacker, struct entity* defender, struct move* move,
                   enum item_id item_id);
bool DoMoveShocker(struct entity* attacker, struct entity* defender, struct move* move,
                   enum item_id item_id);
bool DoMoveEcho(struct entity* attacker, struct entity* defender, struct move* move,
                enum item_id item_id);
bool DoMoveFamish(struct entity* attacker, struct entity* defender, struct move* move,
                  enum item_id item_id);
bool DoMoveOneRoom(struct entity* attacker, struct entity* defender, struct move* move,
                   enum item_id item_id);
bool DoMoveFillIn(struct entity* attacker, struct entity* defender, struct move* move,
                  enum item_id item_id);
bool DoMoveTrapper(struct entity* attacker, struct entity* defender, struct move* move,
                   enum item_id item_id);
bool DoMoveItemize(struct entity* attacker, struct entity* defender, struct move* move,
                   enum item_id item_id);
bool DoMoveHurl(struct entity* attacker, struct entity* defender, struct move* move,
                enum item_id item_id);
bool DoMoveMobile(struct entity* attacker, struct entity* defender, struct move* move,
                  enum item_id item_id);
bool DoMoveSeeStairs(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveLongToss(struct entity* attacker, struct entity* defender, struct move* move,
                    enum item_id item_id);
bool DoMovePierce(struct entity* attacker, struct entity* defender, struct move* move,
                  enum item_id item_id);
bool DoMoveHammerArm(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveAquaRing(struct entity* attacker, struct entity* defender, struct move* move,
                    enum item_id item_id);
bool DoMoveGastroAcid(struct entity* attacker, struct entity* defender, struct move* move,
                      enum item_id item_id);
bool DoMoveHealingWish(struct entity* attacker, struct entity* defender, struct move* move,
                       enum item_id item_id);
bool DoMoveCloseCombat(struct entity* attacker, struct entity* defender, struct move* move,
                       enum item_id item_id);
bool DoMoveLuckyChant(struct entity* attacker, struct entity* defender, struct move* move,
                      enum item_id item_id);
bool DoMoveGuardSwap(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveHealOrder(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveHealBlock(struct entity* attacker, struct entity* defender, struct move* move,
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
bool DoMoveShadowForce(struct entity* attacker, struct entity* defender, struct move* move,
                       enum item_id item_id);
bool DoMoveGravity(struct entity* attacker, struct entity* defender, struct move* move,
                   enum item_id item_id);
bool DoMoveStealthRock(struct entity* attacker, struct entity* defender, struct move* move,
                       enum item_id item_id);
bool DoMoveChargeBeam(struct entity* attacker, struct entity* defender, struct move* move,
                      enum item_id item_id);
bool DoMoveDamageEatItem(struct entity* attacker, struct entity* defender, struct move* move,
                         enum item_id item_id);
bool DoMoveAcupressure(struct entity* attacker, struct entity* defender, struct move* move,
                       enum item_id item_id);
bool DoMoveMagnetRise(struct entity* attacker, struct entity* defender, struct move* move,
                      enum item_id item_id);
bool DoMoveToxicSpikes(struct entity* attacker, struct entity* defender, struct move* move,
                       enum item_id item_id);
bool DoMoveLastResort(struct entity* attacker, struct entity* defender, struct move* move,
                      enum item_id item_id);
bool DoMoveTrickRoom(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveWorrySeed(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveDamageHpDependent(struct entity* attacker, struct entity* defender, struct move* move,
                             enum item_id item_id);
bool DoMoveHeartSwap(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveRoost(struct entity* attacker, struct entity* defender, struct move* move,
                 enum item_id item_id);
bool DoMovePowerSwap(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMovePowerTrick(struct entity* attacker, struct entity* defender, struct move* move,
                      enum item_id item_id);
bool DoMoveFeint(struct entity* attacker, struct entity* defender, struct move* move,
                 enum item_id item_id);
bool DoMoveFlareBlitz(struct entity* attacker, struct entity* defender, struct move* move,
                      enum item_id item_id);
bool DoMoveDefendOrder(struct entity* attacker, struct entity* defender, struct move* move,
                       enum item_id item_id);
bool DoMoveFireFang(struct entity* attacker, struct entity* defender, struct move* move,
                    enum item_id item_id);
bool DoMoveLunarDance(struct entity* attacker, struct entity* defender, struct move* move,
                      enum item_id item_id);
bool DoMoveMiracleEye(struct entity* attacker, struct entity* defender, struct move* move,
                      enum item_id item_id);
bool DoMoveWakeUpSlap(struct entity* attacker, struct entity* defender, struct move* move,
                      enum item_id item_id);
bool DoMoveMetalBurst(struct entity* attacker, struct entity* defender, struct move* move,
                      enum item_id item_id);
bool DoMoveHeadSmash(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveCaptivate(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveLeafStorm(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveDracoMeteor(struct entity* attacker, struct entity* defender, struct move* move,
                       enum item_id item_id, bool disable_spatk_drop);
bool DoMoveRockPolish(struct entity* attacker, struct entity* defender, struct move* move,
                      enum item_id item_id);
bool DoMoveNastyPlot(struct entity* attacker, struct entity* defender, struct move* move,
                     enum item_id item_id);
bool DoMoveTag0x1AB(struct entity* attacker, struct entity* defender, struct move* move,
                    enum item_id item_id);
bool DoMoveTag0x1A6(struct entity* attacker, struct entity* defender, struct move* move,
                    enum item_id item_id);
bool DoMoveTag0x1A7(struct entity* attacker, struct entity* defender, struct move* move,
                    enum item_id item_id);

#endif
