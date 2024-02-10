#ifndef HEADERS_FUNCTIONS_ARM9_LIBS_H_
#define HEADERS_FUNCTIONS_ARM9_LIBS_H_

void DseDriver_LoadDefaultSettings(struct dse_driver_settings* settings);
uint32_t DseDriver_IsSettingsValid(struct dse_driver_settings* settings);
int DseDriver_ConfigureHeap(struct dse_driver_settings* settings, void* location, uint32_t size);
int DseDriver_Init(struct dse_driver_settings* settings);

void Dse_SetError(int error_code, uint32_t data, uint32_t data2);
void Dse_SetError2(int error_code, uint32_t data, uint32_t data2);

uint32_t DseUtil_ByteSwap32(uint32_t value);
int SoundUtil_GetRandomNumber(void);

uint32_t DseMem_Init(void* heap_location, uint32_t heap_size, struct dse_heap_allocator* allocator);
void DseMem_Quit(void);
void* DseMem_AllocateUser(uint32_t size, uint32_t alignment);
void* DseMem_Allocate(uint32_t size, uint32_t alignment, uint32_t label);
void* DseMem_AllocateThreadStack(uint32_t size, uint32_t alignment, uint32_t label);
int DseMem_Free(void* ptr);
void DseMem_Clear(void* ptr, uint32_t size);

int DseFile_CheckHeader(void* file, uint32_t signature, uint16_t version);

void DseSwd_SysInit(uint8_t loader_thread_priority);
void DseSwd_SysQuit(void);
void DseSwd_SampleLoaderMain(void);
int DseSwd_MainBankDummyCallback(struct dse_mainbank_reading* reading, void* arg);
int DseSwd_LoadMainBank(char* path, int max_read_size, void* callback, void* callback_param);
int DseSwd_LoadBank(void* file, uint32_t unused, void* pcm_data_buffer);
int DseSwd_IsBankLoading(void);
int DseSwd_LoadWaves(uint16_t id, void* pcm_data);
int DseSwd_LoadWavesInternal(struct dse_wavebank* bank, void* samples);
int DseSwd_Unload(uint16_t id);
void* ReadWaviEntry(struct wavi_data* wavi_data, int entry_index);
struct dse_instrument* DseSwd_GetInstrument(struct dse_wavebank* bank, int16_t index);
struct dse_instrument_split* DseSwd_GetNextSplitInRange(struct dse_instrument* instrument,
                                                        struct dse_instrument_split* split,
                                                        uint8_t note_num, uint8_t volume);
struct dse_mainbank* DseSwd_GetMainBankById(uint16_t id);
struct dse_wavebank* DseSwd_GetBankById(uint16_t id);
int DseSwd_InitMainBankFileReader(struct file_stream* file);
int DseSwd_OpenMainBankFileReader(struct file_stream* file);
int DseSwd_CloseMainBankFileReader(struct file_stream* file);
int DseSwd_ReadMainBank(struct file_stream* file, void* buffer, int size, int offset,
                        struct dse_mainbank_reading* reading);

int DseBgm_DefaultSignalCallback(int sequence_id, int signal_type, int signal_data,
                                 void* user_param);
int DseBgm_Load(void* file);
int DseBgm_Unload(uint16_t id);
int DseBgm_SetSignalCallback(uint16_t id, void* callback, void* callback_arg);
int DseBgm_IsPlaying(uint16_t id);
int ResumeBgm(undefined4 param_1, undefined4 param_2, undefined4 param_3);
int DseBgm_Stop(uint16_t id, int fadeout_msec);
void DseBgm_StopAll(int fadeout_msec);
int DseBgm_SetFades(uint16_t id, struct dse_fade_params* fade);

int DseSequence_Start(struct dse_sequence* seq, uint16_t unknown, uint8_t effect_id,
                      struct dse_fade_params* fadein);
int DseSequence_PauseList(struct dse_sequence* seq);
void DseSequence_SetFades(struct dse_sequence* seq, struct dse_fade_params* fade);
int DseSequence_GetParameter(uint16_t id, int which, int* out);
uint32_t DseSequence_GetSmallestNumLoops(struct dse_sequence* seq);
int DseSequence_Reset(struct dse_sequence* seq);
int DseSequence_Stop(struct dse_sequence* seq, int fadeout_msec, int is_effect_sequence);
void* FindSmdlSongChunk(void* smdl_data, uint16_t value_to_search);
int DseSequence_LoadSong(struct dse_sequence* seq, struct dse_song* song);
struct dse_sequence* DseSequence_GetById(uint16_t id);
struct dse_sequence* DseSequence_AllocateNew(uint8_t num_tracks, uint8_t num_channels,
                                             uint32_t type_label);
int DseSequence_Unload(struct dse_sequence* seq);
void DseSequence_InitTracks(struct dse_sequence* seq);
void DseBgm_SysSetupNoteList(void);

int DseSe_SysReset(uint16_t num_sequences, uint16_t tracks_per_sequence);
int DseSe_Load(void* file);
int DseSe_Unload(uint16_t id);
int DseSe_GetUsedBankIDs(uint16_t id, uint16_t* wavebank_ids, int max_num_wavebank_ids);
int DseSe_HasPlayingInstances(uint16_t id, uint32_t unknown);
int DseSe_Play(uint16_t id, uint16_t unknown, struct dse_fade_params* fadein);
struct dse_song* DseSe_GetEffectSong(void* file, int song_index, void* seq_chunk_data);
int DseSe_CheckTooManyInstances(uint16_t id, int max_instances, int8_t unknown);
struct dse_se_bank* DseSe_GetById(uint16_t id);

int FlushChannels(undefined* param_1, int param_2, int param_3);

int DseDriver_StartMainThread(void);
int DseDriver_StartTickTimer(void);
void DseDriver_NotifyTick(void);
void DseDriver_Main(void);
void DseSequence_TickNotes(struct dse_sequence* sequence);
void ParseDseEvent(undefined* audio_state, struct track_data* track_data);
void UpdateSequencerTracks(int param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4);
void DseSequence_TickFades(struct dse_sequence* seq);
uint8_t* DseTrackEvent_Invalid(uint8_t* ptr_next_byte, struct dse_sequence* sequence,
                               struct dse_track* track, struct dse_channel* channel);
uint8_t* DseTrackEvent_WaitSame(uint8_t* ptr_next_byte, struct dse_sequence* sequence,
                                struct dse_track* track, struct dse_channel* channel);
uint8_t* DseTrackEvent_WaitDelta(uint8_t* ptr_next_byte, struct dse_sequence* sequence,
                                 struct dse_track* track, struct dse_channel* channel);
uint8_t* DseTrackEvent_Wait8(uint8_t* ptr_next_byte, struct dse_sequence* sequence,
                             struct dse_track* track, struct dse_channel* channel);
uint8_t* DseTrackEvent_Wait16(uint8_t* ptr_next_byte, struct dse_sequence* sequence,
                              struct dse_track* track, struct dse_channel* channel);
uint8_t* DseTrackEvent_Wait24(uint8_t* ptr_next_byte, struct dse_sequence* sequence,
                              struct dse_track* track, struct dse_channel* channel);
uint8_t* DseTrackEvent_WaitUntilFadeout(uint8_t* ptr_next_byte, struct dse_sequence* sequence,
                                        struct dse_track* track, struct dse_channel* channel);
uint8_t* DseTrackEvent_EndTrack(uint8_t* ptr_next_byte, struct dse_sequence* sequence,
                                struct dse_track* track, struct dse_channel* channel);
uint8_t* DseTrackEvent_MainLoopBegin(uint8_t* ptr_next_byte, struct dse_sequence* sequence,
                                     struct dse_track* track, struct dse_channel* channel);
uint8_t* DseTrackEvent_SubLoopBegin(uint8_t* ptr_next_byte, struct dse_sequence* sequence,
                                    struct dse_track* track, struct dse_channel* channel);
uint8_t* DseTrackEvent_SubLoopEnd(uint8_t* ptr_next_byte, struct dse_sequence* sequence,
                                  struct dse_track* track, struct dse_channel* channel);
uint8_t* DseTrackEvent_SubLoopBreakOnLastIteration(uint8_t* ptr_next_byte,
                                                   struct dse_sequence* sequence,
                                                   struct dse_track* track,
                                                   struct dse_channel* channel);
uint8_t* DseTrackEvent_SetOctave(uint8_t* ptr_next_byte, struct dse_sequence* sequence,
                                 struct dse_track* track, struct dse_channel* channel);
uint8_t* DseTrackEvent_OctaveDelta(uint8_t* ptr_next_byte, struct dse_sequence* sequence,
                                   struct dse_track* track, struct dse_channel* channel);
uint8_t* DseTrackEvent_SetBpm(uint8_t* ptr_next_byte, struct dse_sequence* sequence,
                              struct dse_track* track, struct dse_channel* channel);
uint8_t* DseTrackEvent_SetBpm2(uint8_t* ptr_next_byte, struct dse_sequence* sequence,
                               struct dse_track* track, struct dse_channel* channel);
uint8_t* DseTrackEvent_SetBank(uint8_t* ptr_next_byte, struct dse_sequence* sequence,
                               struct dse_track* track, struct dse_channel* channel);
uint8_t* DseTrackEvent_SetBankMsb(uint8_t* ptr_next_byte, struct dse_sequence* sequence,
                                  struct dse_track* track, struct dse_channel* channel);
uint8_t* DseTrackEvent_SetBankLsb(uint8_t* ptr_next_byte, struct dse_sequence* sequence,
                                  struct dse_track* track, struct dse_channel* channel);
uint8_t* DseTrackEvent_Dummy1Byte(uint8_t* ptr_next_byte, struct dse_sequence* sequence,
                                  struct dse_track* track, struct dse_channel* channel);
uint8_t* DseTrackEvent_SetInstrument(uint8_t* ptr_next_byte, struct dse_sequence* sequence,
                                     struct dse_track* track, struct dse_channel* channel);
uint8_t* DseTrackEvent_SongVolumeFade(uint8_t* ptr_next_byte, struct dse_sequence* sequence,
                                      struct dse_track* track, struct dse_channel* channel);
uint8_t* DseTrackEvent_RestoreEnvelopeDefaults(uint8_t* ptr_next_byte,
                                               struct dse_sequence* sequence,
                                               struct dse_track* track,
                                               struct dse_channel* channel);
uint8_t* DseTrackEvent_SetEnvelopeAttackBegin(uint8_t* ptr_next_byte, struct dse_sequence* sequence,
                                              struct dse_track* track, struct dse_channel* channel);
uint8_t* DseTrackEvent_SetEnvelopeAttackTime(uint8_t* ptr_next_byte, struct dse_sequence* sequence,
                                             struct dse_track* track, struct dse_channel* channel);
uint8_t* DseTrackEvent_SetEnvelopeHoldTime(uint8_t* ptr_next_byte, struct dse_sequence* sequence,
                                           struct dse_track* track, struct dse_channel* channel);
uint8_t* DseTrackEvent_SetEnvelopeDecayTimeAndSustainLevel(uint8_t* ptr_next_byte,
                                                           struct dse_sequence* sequence,
                                                           struct dse_track* track,
                                                           struct dse_channel* channel);
uint8_t* DseTrackEvent_SetEnvelopeSustainTime(uint8_t* ptr_next_byte, struct dse_sequence* sequence,
                                              struct dse_track* track, struct dse_channel* channel);
uint8_t* DseTrackEvent_SetEnvelopeReleaseTime(uint8_t* ptr_next_byte, struct dse_sequence* sequence,
                                              struct dse_track* track, struct dse_channel* channel);
uint8_t* DseTrackEvent_SetNoteDurationMultiplier(uint8_t* ptr_next_byte,
                                                 struct dse_sequence* sequence,
                                                 struct dse_track* track,
                                                 struct dse_channel* channel);
uint8_t* DseTrackEvent_ForceLfoEnvelopeLevel(uint8_t* ptr_next_byte, struct dse_sequence* sequence,
                                             struct dse_track* track, struct dse_channel* channel);
uint8_t* DseTrackEvent_SetHoldNotes(uint8_t* ptr_next_byte, struct dse_sequence* sequence,
                                    struct dse_track* track, struct dse_channel* channel);
uint8_t* DseTrackEvent_SetFlagBit1Unknown(uint8_t* ptr_next_byte, struct dse_sequence* sequence,
                                          struct dse_track* track, struct dse_channel* channel);
uint8_t* DseTrackEvent_SetOptionalVolume(uint8_t* ptr_next_byte, struct dse_sequence* sequence,
                                         struct dse_track* track, struct dse_channel* channel);
uint8_t* DseTrackEvent_Dummy2Bytes(uint8_t* ptr_next_byte, struct dse_sequence* sequence,
                                   struct dse_track* track, struct dse_channel* channel);
uint8_t* DseTrackEvent_SetTuning(uint8_t* ptr_next_byte, struct dse_sequence* sequence,
                                 struct dse_track* track, struct dse_channel* channel);
uint8_t* DseTrackEvent_TuningDeltaCoarse(uint8_t* ptr_next_byte, struct dse_sequence* sequence,
                                         struct dse_track* track, struct dse_channel* channel);
uint8_t* DseTrackEvent_TuningDeltaFine(uint8_t* ptr_next_byte, struct dse_sequence* sequence,
                                       struct dse_track* track, struct dse_channel* channel);
uint8_t* DseTrackEvent_TuningDeltaFull(uint8_t* ptr_next_byte, struct dse_sequence* sequence,
                                       struct dse_track* track, struct dse_channel* channel);
uint8_t* DseTrackEvent_TuningFade(uint8_t* ptr_next_byte, struct dse_sequence* sequence,
                                  struct dse_track* track, struct dse_channel* channel);
uint8_t* DseTrackEvent_SetNoteRandomRegion(uint8_t* ptr_next_byte, struct dse_sequence* sequence,
                                           struct dse_track* track, struct dse_channel* channel);
uint8_t* DseTrackEvent_SetTuningJitterAmplitude(uint8_t* ptr_next_byte,
                                                struct dse_sequence* sequence,
                                                struct dse_track* track,
                                                struct dse_channel* channel);
uint8_t* DseTrackEvent_SetKeyBend(uint8_t* ptr_next_byte, struct dse_sequence* sequence,
                                  struct dse_track* track, struct dse_channel* channel);
uint8_t* DseTrackEvent_SetUnknown2(uint8_t* ptr_next_byte, struct dse_sequence* sequence,
                                   struct dse_track* track, struct dse_channel* channel);
uint8_t* DseTrackEvent_SetKeyBendRange(uint8_t* ptr_next_byte, struct dse_sequence* sequence,
                                       struct dse_track* track, struct dse_channel* channel);
uint8_t* DseTrackEvent_SetupKeyBendLfo(uint8_t* ptr_next_byte, struct dse_sequence* sequence,
                                       struct dse_track* track, struct dse_channel* channel);
uint8_t* DseTrackEvent_SetupKeyBendLfoEnvelope(uint8_t* ptr_next_byte,
                                               struct dse_sequence* sequence,
                                               struct dse_track* track,
                                               struct dse_channel* channel);
uint8_t* DseTrackEvent_UseKeyBendLfo(uint8_t* ptr_next_byte, struct dse_sequence* sequence,
                                     struct dse_track* track, struct dse_channel* channel);
uint8_t* DseTrackEvent_SetVolume(uint8_t* ptr_next_byte, struct dse_sequence* sequence,
                                 struct dse_track* track, struct dse_channel* channel);
uint8_t* DseTrackEvent_VolumeDelta(uint8_t* ptr_next_byte, struct dse_sequence* sequence,
                                   struct dse_track* track, struct dse_channel* channel);
uint8_t* DseTrackEvent_VolumeFade(uint8_t* ptr_next_byte, struct dse_sequence* sequence,
                                  struct dse_track* track, struct dse_channel* channel);
uint8_t* DseTrackEvent_SetExpression(uint8_t* ptr_next_byte, struct dse_sequence* sequence,
                                     struct dse_track* track, struct dse_channel* channel);
uint8_t* DseTrackEvent_SetupVolumeLfo(uint8_t* ptr_next_byte, struct dse_sequence* sequence,
                                      struct dse_track* track, struct dse_channel* channel);
uint8_t* DseTrackEvent_SetupVolumeLfoEnvelope(uint8_t* ptr_next_byte, struct dse_sequence* sequence,
                                              struct dse_track* track, struct dse_channel* channel);
uint8_t* DseTrackEvent_UseVolumeLfo(uint8_t* ptr_next_byte, struct dse_sequence* sequence,
                                    struct dse_track* track, struct dse_channel* channel);
uint8_t* DseTrackEvent_SetPan(uint8_t* ptr_next_byte, struct dse_sequence* sequence,
                              struct dse_track* track, struct dse_channel* channel);
uint8_t* DseTrackEvent_PanDelta(uint8_t* ptr_next_byte, struct dse_sequence* sequence,
                                struct dse_track* track, struct dse_channel* channel);
uint8_t* DseTrackEvent_PanFade(uint8_t* ptr_next_byte, struct dse_sequence* sequence,
                               struct dse_track* track, struct dse_channel* channel);
uint8_t* DseTrackEvent_SetupPanLfo(uint8_t* ptr_next_byte, struct dse_sequence* sequence,
                                   struct dse_track* track, struct dse_channel* channel);
uint8_t* DseTrackEvent_SetupPanLfoEnvelope(uint8_t* ptr_next_byte, struct dse_sequence* sequence,
                                           struct dse_track* track, struct dse_channel* channel);
uint8_t* DseTrackEvent_UsePanLfo(uint8_t* ptr_next_byte, struct dse_sequence* sequence,
                                 struct dse_track* track, struct dse_channel* channel);
uint8_t* DseTrackEvent_SetupLfo(uint8_t* ptr_next_byte, struct dse_sequence* sequence,
                                struct dse_track* track, struct dse_channel* channel);
uint8_t* DseTrackEvent_SetupLfoEnvelope(uint8_t* ptr_next_byte, struct dse_sequence* sequence,
                                        struct dse_track* track, struct dse_channel* channel);
uint8_t* DseTrackEvent_SetLfoParameter(uint8_t* ptr_next_byte, struct dse_sequence* sequence,
                                       struct dse_track* track, struct dse_channel* channel);
uint8_t* DseTrackEvent_UseLfo(uint8_t* ptr_next_byte, struct dse_sequence* sequence,
                              struct dse_track* track, struct dse_channel* channel);
uint8_t* DseTrackEvent_Signal(uint8_t* ptr_next_byte, struct dse_sequence* sequence,
                              struct dse_track* track, struct dse_channel* channel);
uint8_t* DseTrackEvent_Dummy2Bytes2(uint8_t* ptr_next_byte, struct dse_sequence* sequence,
                                    struct dse_track* track, struct dse_channel* channel);

void DseSynth_Reset(struct dse_synth* synth);
struct dse_synth* DseSynth_AllocateNew(int num_channels, uint32_t label);
void DseSynth_Unload(struct dse_synth* synth);
void DseSynth_ClearHeldNotes(struct dse_synth* synth);
void DseSynth_ResetAndSetBankAndSequence(struct dse_synth* synth, uint16_t bank_id,
                                         uint16_t sequence_id);
void DseSynth_StopChannels(struct dse_synth* synth);
void DseSynth_ResetAllVoiceTimersAndVolumes(struct dse_synth* synth, int clear_volume_and_timer);
void DseSynth_RestoreHeldNotes(struct dse_synth* synth);
void DseSynth_SetGlobalVolumeIndex(struct dse_synth* synth, uint32_t global_volume_index,
                                   int notify_update);
void DseSynth_SetBend(struct dse_synth* synth, uint16_t bend);
void DseSynth_SetVolume(struct dse_synth* synth, int8_t volume);
void DseSynth_SetPan(struct dse_synth* synth, int8_t pan);
void DseSynth_SetBankAndSequence(struct dse_synth* synth, uint16_t bank_id, uint16_t sequence_id);

void DseChannel_Init(struct dse_channel* channel, uint32_t sequence_id);
void DseChannel_DeallocateVoices(struct dse_channel* channel);
void DseChannel_ResetTimerAndVolumeForVoices(struct dse_channel* channel,
                                             int clear_volume_and_timer);
void DseChannel_SetBank(struct dse_channel* channel, uint16_t swd_id);
int DseChannel_SetInstrument(struct dse_channel* channel, int16_t instrument_index);
void DseChannel_SetKeyBend(struct dse_channel* channel, int16_t bend);
struct dse_note* DseChannel_AllocateNote(struct dse_channel* channel, struct dse_note* note);
void DseChannel_ReleaseNoteInternal(struct dse_channel* channel, struct dse_note* note);
void DseChannel_ChangeNote(struct dse_channel* channel, struct dse_note* note,
                           uint32_t note_number);
void DseChannel_ReleaseNote(struct dse_channel* channel, struct dse_note* note);

void DseVoice_PlayNote(struct dse_channel* channel, struct dse_note* note);
void DseVoice_ReleaseNote(struct dse_channel* channel, struct dse_note* note);
void DseVoice_UpdateParameters(void);
void DseVoice_ResetAll(void);
void DseVoice_ResetHW(int16_t num_voices);
void UpdateChannels(void);
struct dse_voice* DseVoice_Allocate(struct dse_channel* channel, struct dse_keygroup* keygroup,
                                    uint32_t channel_and_keygroup_indices, int keygroup_priority);
void DseVoice_Start(struct dse_channel* channel, struct dse_voice* voice, int group_priority);
void DseVoice_ReleaseHeld(struct dse_channel* channel);
void DseVoice_Release(struct dse_channel* channel, struct dse_voice* voice);
void DseVoice_Deallocate(struct dse_voice* voice);
void DseVoice_FlagForActivation(struct dse_voice* voice);
void DseVoice_FlagForDeactivation(struct dse_voice* voice);
int DseVoice_CountNumActiveInChannel(struct dse_channel* channel);
void DseVoice_UpdateHardware(void);

void SoundEnvelope_Reset(struct sound_envelope* envelope);
void SoundEnvelopeParameters_Reset(struct sound_envelope_parameters* parameters);
void SoundEnvelopeParameters_CheckValidity(struct sound_envelope_parameters* parameters);
void SoundEnvelope_SetParameters(struct sound_envelope* envelope,
                                 struct sound_envelope_parameters* parameters);
void SoundEnvelope_SetSlide(struct sound_envelope* envelope, int target_volume, int msec_tab_index);
void UpdateTrackVolumeEnvelopes(struct sound_envelope* envelope);
void SoundEnvelope_Release(struct sound_envelope* envelope);
void SoundEnvelope_Stop(struct sound_envelope* envelope);
void SoundEnvelope_ForceVolume(struct sound_envelope* envelope, int volume);
void SoundEnvelope_Stop2(struct sound_envelope* envelope);
int8_t SoundEnvelope_Tick(struct sound_envelope* envelope);
void SoundLfoBank_Reset(struct dse_lfo_bank* lfo_bank);
void SoundLfoBank_Set(struct dse_lfo_bank* lfo_bank, struct dse_lfo_settings* lfo_settings,
                      int8_t const_envelope_level);
void SoundLfoBank_SetConstEnvelopes(struct dse_lfo_bank* lfo_bank, int8_t level);
uint16_t SoundLfoBank_Tick(struct dse_lfo_bank* lfo_bank);
int SoundLfoWave_InvalidFunc(struct dse_lfo* lfo);
int SoundLfoWave_HalfSquareFunc(struct dse_lfo* lfo);
int SoundLfoWave_FullSquareFunc(struct dse_lfo* lfo);
int SoundLfoWave_HalfTriangleFunc(struct dse_lfo* lfo);
int SoundLfoWave_FullTriangleFunc(struct dse_lfo* lfo);
int SoundLfoWave_SawFunc(struct dse_lfo* lfo);
int SoundLfoWave_ReverseSawFunc(struct dse_lfo* lfo);
int SoundLfoWave_HalfNoiseFunc(struct dse_lfo* lfo);
int SoundLfoWave_FullNoiseFunc(struct dse_lfo* lfo);
void EnableVramBanksInSetDontSave(struct vram_banks_set vram_banks);
void EnableVramBanksInSet(struct vram_banks_set* vram_banks);
void G3_LoadMtx43(struct matrix_4x3* matrix);
void G3_MultMtx43(struct matrix_4x3* matrix);
void GeomGxFifoSendMtx4x3(struct matrix_4x3* matrix, void* gxfifo);
int IncrementThreadCount(void);
void InsertThreadIntoList(struct thread* thread);
void StartThread(struct thread* thread, thread_entry_fn_t function, struct thread* param_3,
                 void* stack_area, int stack_size, int param_6);
void ThreadExit(void);
void SetThreadField0xB4(struct thread* thread, int value);
void InitThread(struct thread* thread, void* function, void* stack_area_minus_4);
uint16_t GetTimer0Control(void);
int ClearIrqFlag(void);
int EnableIrqFlag(void);
int SetIrqFlag(int new_value);
int EnableIrqFiqFlags(void);
int SetIrqFiqFlags(int new_value);
int GetIrqFlag(void);
int GetProcessorMode(void);
uint32_t CountLeadingZeros(uint32_t x);
void WaitForever2(void);
void WaitForInterrupt(void);
void ArrayFill16(uint16_t val, void* ptr, int len);
void ArrayCopy16(void* src, void* dest, int len);
void ArrayFill32(uint32_t val, void* ptr, int len);
void ArrayCopy32(void* src, void* dest, int len);
void ArrayFill32Fast(uint32_t val, void* ptr, int len);
void ArrayCopy32Fast(void* src, void* dest, int len);
void MemsetFast(void* ptr, char val, uint32_t len);
void MemcpyFast(void* src, void* dest, uint32_t n);
uint32_t AtomicExchange(uint32_t desired, void* ptr);
void FileInit(struct file_stream* file);
bool GetOverlayInfo(struct overlay_info_entry* overlay_info, undefined param_2, int overlay_id);
bool LoadOverlayInternal(struct overlay_info_entry* overlay_info);
void InitOverlay(struct overlay_info_entry* overlay_info);

// If declaring these builtins causes issues, you can disable them
#ifndef PMDSKY_NO_BUILTIN
int abs(int x);
int mbtowc(wchar_t* pwc, const char* s, size_t n);
bool TryAssignByte(uint8_t* ptr, uint8_t val);
bool TryAssignByteWrapper(uint8_t* ptr, uint8_t val);
size_t wcstombs(char* dest, const wchar_t* src, size_t n);
void* memcpy(void* dest, const void* src, size_t n);
void* memmove(void* dest, const void* src, size_t n);
void* memset(void* s, int c, size_t n);
void* memchr(const void* s, int c, size_t n);
int memcmp(const void* s1, const void* s2, size_t n);
void memset_internal(void* s, int c, size_t n);
int __vsprintf_internal_slice(slice_append_fn_t append, struct slice* str, const char* format,
                              va_list ap);
bool TryAppendToSlice(struct slice* slice, const void* data, size_t data_len);
int __vsprintf_internal(char* str, size_t maxlen, const char* format, va_list ap);
int vsprintf(char* str, const char* format, va_list ap);
int snprintf(char* str, size_t n, const char* format, ...);
int sprintf(char* str, const char* format, ...);
size_t strlen(const char* s);
char* strcpy(char* dest, const char* src);
char* strncpy(char* dest, const char* src, size_t n);
char* strcat(char* dest, const char* src);
char* strncat(char* dest, const char* src, size_t n);
int strcmp(const char* s1, const char* s2);
int strncmp(const char* s1, const char* s2, size_t n);
char* strchr(const char* string, int c);
size_t strcspn(const char* string, const char* stopset);
char* strstr(const char* haystack, const char* needle);
size_t wcslen(const wchar_t* ws);
double _dadd(double a, double b);
float _d2f(double d);
unsigned long long _ll_ufrom_d(double d);
double _dflt(int i);
double _dfltu(uint32_t u);
double _dmul(double a, double b);
double _dsqrt(double x);
double _dsub(double a, double b);
float _fadd(float a, float b);
bool _dgeq(double a, double b);
bool _dleq(double a, double b);
bool _dls(double a, double b);
bool _deq(double a, double b);
bool _dneq(double a, double b);
bool _fls(float a, float b);
float _fdiv(float dividend, float divisor);
double _f2d(float f);
int _ffix(float f);
float _fflt(int i);
float _ffltu(uint32_t u);
float _fmul(float a, float b);
float sqrtf(float x);
float _fsub(float a, float b);
long long _ll_mod(long long dividend, long long divisor);
long long _ll_sdiv(long long dividend, long long divisor);
unsigned long long _ll_udiv(unsigned long long dividend, unsigned long long divisor);
unsigned long long _ull_mod(unsigned long long dividend, unsigned long long divisor);
long long _ll_mul(long long a, long long b);
unsigned long long _s32_div_f(int dividend, int divisor);
unsigned long long _u32_div_f(uint32_t dividend, uint32_t divisor);
unsigned long long _u32_div_not_0_f(uint32_t dividend, uint32_t divisor);
double _drdiv(double divisor, double dividend);
double _ddiv(double dividend, double divisor);
void _fp_init(void);
#endif // #ifndef PMDSKY_NO_BUILTIN

#endif
