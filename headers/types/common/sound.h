// Types used by the DSE sound library.

#ifndef HEADERS_TYPES_COMMON_SOUND_H_
#define HEADERS_TYPES_COMMON_SOUND_H_

#include "enums.h"
#include "file_io.h"
#include "util.h"

struct sound_envelope_parameters {
    uint8_t use_envelope;
    uint8_t slide_time_multiplier;
    uint16_t field_0x2;
    uint32_t field_0x4;
    uint8_t attack_begin;
    uint8_t attack_time;
    uint8_t decay_time;
    uint8_t sustain_level;
    uint8_t hold_time;
    uint8_t sustain_time;
    uint8_t release_time;
    uint8_t field_0xe;
};
ASSERT_SIZE(struct sound_envelope_parameters, 16);

struct sound_envelope {
    struct sound_envelope_parameters parameters;
    int current_volume;
    int volume_delta;
    int ticks_left;
    uint8_t state;
    uint8_t target_volume;
    uint8_t update_volume;
    uint8_t _unused;
};
ASSERT_SIZE(struct sound_envelope, 32);

struct dse_lfo;

typedef int (*sound_lfo_waveform_callback)(struct dse_lfo* lfo);

struct dse_lfo {
    uint8_t phase_flags;
    uint8_t type;
    uint16_t voice_update_flags;
    uint16_t ticks_per_phase_change;
    uint16_t ticks_until_phase_change;
    int current_output;
    int amplitude;
    uint32_t field_0x10;
    int output_delta;
    uint16_t field_0x18;
    uint16_t ticks_until_lfo_started;
    uint16_t field_0x1c;
    uint16_t lfo_envelope_ticks_left;
    int envelope_level;
    int envelope_delta;
    int16_t* output_ptr;
    sound_lfo_waveform_callback waveform_callback;
    struct dse_lfo* next;
};
ASSERT_SIZE(struct dse_lfo, 52);

struct dse_lfo_bank {
    int16_t outputs[6];
    struct dse_lfo* lfo_list;
    struct dse_lfo lfos[4]; // Size: 4 * 0x34 = 0xd0
};
ASSERT_SIZE(struct dse_lfo_bank, 224);

struct dse_lfo_settings {
    uint8_t field_0x0;
    uint8_t type;
    uint8_t output_type;
    uint8_t lfo_waveform_index;
    int amplitude;
    uint16_t lfo_phase_change_msec;
    uint16_t msec_until_lfo_started;
    uint16_t lfo_envelope_len_msec;
    uint16_t _unused;
};
ASSERT_SIZE(struct dse_lfo_settings, 16);

struct dse_voice_update_flags {
    uint16_t flags[8];
};
ASSERT_SIZE(struct dse_voice_update_flags, 16);

// Used as a parameter to SendAudioCommand. Includes data on which audio to play and how.
struct audio_command {
    // 0x0: Seems to be a value that marks the status of this entry. It's probably an enum, maybe a
    // command ID. Seems to be 0 when the entry is not in use.
    int status;
    struct music_id_16 music_id; // 0x4: ID of the music to play
    uint16_t volume;             // 0x6: Volume (0-255)
    undefined2 field_0x8;
    undefined field_0xA;
    undefined field_0xB;
    undefined field_0xC;
    undefined field_0xD;
    undefined field_0xE;
    undefined field_0xF;
    undefined field_0x10;
    undefined field_0x11;
    undefined field_0x12;
    undefined field_0x13;
    undefined field_0x14;
    undefined field_0x15;
    undefined field_0x16;
    undefined field_0x17;
    undefined field_0x18;
    undefined field_0x19;
    undefined field_0x1A;
    undefined field_0x1B;
    undefined field_0x1C;
    undefined field_0x1D;
    undefined field_0x1E;
    undefined field_0x1F;
};
ASSERT_SIZE(struct audio_command, 32);

// Contains data for an audio track that is being played
struct track_data {
    undefined field_0x0;
    undefined field_0x1;
    bool active; // 0x2: True if the track is active
    undefined field_0x3;
    undefined field_0x4;
    undefined field_0x5;
    undefined field_0x6;
    undefined field_0x7;
    int play_amount; // 0x8: Number of times the track has been played so far
    // 0xC: Delay (in frames, probably) before the next DSE event on this track begins
    int event_delay;
    undefined field_0x10;
    undefined field_0x11;
    undefined field_0x12;
    undefined field_0x13;
    undefined field_0x14;
    undefined field_0x15;
    undefined field_0x16;
    undefined field_0x17;
    void* track_data_start; // 0x18: Pointer to the start of the track's audio data
    void* current_event;    // 0x1C: Pointer to the current DSE event
};
ASSERT_SIZE(struct track_data, 32); // Exact size hasn't been confirmed

// Data about a wavi container
struct wavi_data {
    undefined field_0x0;
    undefined field_0x1;
    undefined field_0x2;
    undefined field_0x3;
    undefined field_0x4;
    undefined field_0x5;
    undefined field_0x6;
    undefined field_0x7;
    int16_t num_entries; // 0x8: Number of entries in the container
    undefined field_0xA;
    undefined field_0xB;
    undefined field_0xC;
    undefined field_0xD;
    undefined field_0xE;
    undefined field_0xF;
    void* pointer_table_start; // 0x10: Pointer to the start of the pointer table
};
ASSERT_SIZE(struct wavi_data, 20); // Likely longer

struct dse_fade {
    int current;
    int delta;
    int target;
    int16_t ticks_remaining;
    uint16_t padding;
};
ASSERT_SIZE(struct dse_fade, 16);

struct dse_fade_params {
    uint8_t use_fade_flags;
    uint8_t field_0x2;
    int16_t unknown_msecs;
    int16_t tempo_target;
    uint16_t tempo_fade_msec;
    int16_t bend_target;
    uint16_t bend_fade_msec;
    int16_t volume_target;
    int16_t volume_fade_msec;
    uint16_t pan_target;
    uint16_t pan_fade_msec;
};
ASSERT_SIZE(struct dse_fade_params, 20);

struct dse_channel {
    uint32_t sequence_id;
    uint8_t channel_flags;
    uint8_t channel_index;
    uint8_t field_0x7;
    uint8_t field_0x8;
    uint8_t field_0x9;
    int8_t total_num_voices;
    int16_t bend_jitter_amplitude;
    uint8_t note_random_region_begin;
    uint8_t note_random_region_end;
    uint16_t swd_id;
    int16_t selected_instrument_index;
    int16_t field_0x14;
    int16_t field_0x16;
    uint16_t bend_final;
    uint16_t volume_final;
    int16_t pan_final;
    struct dse_fade bend;
    struct dse_fade volume;
    struct dse_fade pan;
    uint32_t field_0x4C;
    uint8_t expression;
    uint8_t lfo_amplitude;
    int16_t key_bend;
    int16_t tuning;
    int16_t field_0x56;
    uint8_t optional_volume;
    uint8_t bend_sensitivity_override;
    uint8_t field_0x5A[10];
    struct sound_envelope_parameters envelope_override;
    struct dse_lfo_settings lfo_settings[4];

    struct dse_voice* voice_list;
    struct dse_note_list* held_notes_list;
    struct dse_wavebank* wavebank;
    struct dse_instrument* instrument;
    struct dse_synth* synth;
};
ASSERT_SIZE(struct dse_channel, 200);

struct dse_file_chunk {
    uint32_t signature;
    uint32_t field_0x4;
    uint8_t padding_alignment;
    uint8_t field_0x9;
    uint16_t field_0xA;
    uint32_t data_size;
};
ASSERT_SIZE(struct dse_file_chunk, 16);

struct dse_file_header {
    uint32_t signature;
    uint32_t field_0x4;
    uint32_t field_0x8;
    uint16_t version;
    uint16_t id;
    uint8_t field_0x10[16];
    uint8_t file_name[16];
};
ASSERT_SIZE(struct dse_file_header, 48);

struct dse_heap_allocator {
    void* allocate_fun;
    void* free_fun;
    void* arg;
};
ASSERT_SIZE(struct dse_heap_allocator, 12);

struct dse_instrument_split {
    uint8_t field_0x0;
    uint8_t index;
    uint8_t bend_sensitivity;
    uint8_t field_0x3;
    uint8_t min_note_num;
    uint8_t max_note_num;
    uint8_t field_0x6;
    uint8_t field_0x7;
    uint8_t min_note_volume;
    uint8_t max_note_volume;
    uint8_t field_0xC[8];
    int16_t wave_index;
    int16_t key_base;
    uint8_t field_0x16;
    int8_t note_delta;
    uint8_t volume;
    uint8_t pan;
    uint8_t keygroup_index;
    uint8_t field_0x1B[5];

    struct sound_envelope_parameters envelope_params;
};
ASSERT_SIZE(struct dse_instrument_split, 48);

struct dse_instrument {
    uint16_t field_0x0;
    uint8_t num_splits;
    uint8_t field_0x3;
    uint8_t volume;
    uint8_t field_0x5[11];

    struct dse_lfo_settings lfo_settings;
    uint8_t field_0x20[16];

    struct dse_instrument_split splits[];
};
ASSERT_SIZE(struct dse_instrument, 48);

struct dse_keygroup {
    uint8_t index;
    uint8_t field_0x1;
    uint8_t max_polyphony;
    uint8_t priority;
    uint8_t min_voice_index;
    uint8_t max_voice_index;
    uint8_t field_0x6;
    uint8_t field_0x7;
};
ASSERT_SIZE(struct dse_keygroup, 8);

struct dse_mainbank {
    uint16_t id;
    uint16_t num_wavi;
    uint32_t pcm_data_offset;
    void* wavi_data;
    uint32_t field_0xC;
    uint32_t field_0x10;

    struct file_stream file;

    struct dse_mainbank* next;
};
ASSERT_SIZE(struct dse_mainbank, 96);

struct dse_mainbank_reading {
    int size;
    int size_plus_loaded_size;
    int want_size;
    int loaded_size;
};
ASSERT_SIZE(struct dse_mainbank_reading, 16);

struct dse_note {
    uint8_t is_note_on;
    uint8_t is_held;
    uint8_t note_number;
    int8_t volume;
    int duration;
};
ASSERT_SIZE(struct dse_note, 8);

struct dse_note_list_node {
    struct dse_note note;
    struct dse_note_list_node* next;
};
ASSERT_SIZE(struct dse_note_list_node, 12);

struct dse_sample {
    uint8_t field_0x0[3];
    uint8_t sample_format;
    uint8_t field_0x4[5];
    uint8_t psg_duty;
    uint8_t field_0xA[10];
    void* sample_pcm_data;
    int sample_loop_start;
    int sample_size;
};
ASSERT_SIZE(struct dse_sample, 32);

struct dse_se_bank {
    void* file;
    int16_t field_0x4;
    uint16_t id;
    struct dse_se_bank* next;
    void* seq_location;
    void* mcrl_location;
    void* bnkl_location;

    uint8_t field_0x18[4];
};
ASSERT_SIZE(struct dse_se_bank, 28);

struct dse_song {
    int16_t song_id;
    uint16_t ticks_per_beat;
    uint8_t field_0x4;
    uint8_t field_0x5;
    uint8_t num_tracks;
    uint8_t num_channels;
    uint32_t field_0x8;
    uint32_t field_0xC;
    uint32_t field_0x10;
    uint32_t field_0x14;
    uint8_t flags;
    uint8_t global_volume_index;
    uint8_t effect_group_id;
    uint8_t field_0x17; // Something to do with sound effects
    uint8_t max_effect_instances;
    uint8_t field_0x19[19];
};
ASSERT_SIZE(struct dse_song, 48);

struct dse_swd_file_header {
    struct dse_file_header base_file_header;
    uint8_t field_0x30[16];
    uint16_t mainbank_id;
    uint8_t field_0x42[4];
    uint16_t num_waves;
    uint16_t num_instruments; // a.k.a. programs or presets
    uint8_t num_keygroups;
    uint8_t sample_container_kind;
    uint32_t wavi_data_size;
};
ASSERT_SIZE(struct dse_swd_file_header, 80);

struct dse_synth {
    uint8_t field_0x0;
    uint8_t num_channels;
    int8_t global_volume;
    int8_t global_volume_index;
    uint16_t bend; // Or maybe tuning../
    int8_t volume;
    int8_t pan;
    int8_t song_and_global_volume;
    uint8_t clear_volume_and_timer;
    uint8_t field_0xA;
    uint8_t num_voices;
    struct dse_synth* next;
    struct dse_channel channels[];
};
ASSERT_SIZE(struct dse_synth, 16);

struct dse_track_loop {
    uint8_t* start_pos;
    uint8_t* continue_pos;
    uint8_t loops_left;
    uint8_t padding[3];
};
ASSERT_SIZE(struct dse_track_loop, 12);

struct dse_track {
    uint16_t field_0x0;
    int8_t is_playing;
    uint8_t field_0x3;
    uint8_t current_octave;
    uint8_t note_duration_multiplier;
    uint8_t loop_stack_index;
    uint8_t previous_note_number;
    uint8_t ignore_playnote;
    uint8_t field_0x9;
    uint16_t main_loop_count;
    int wait_ticks_left;
    int previous_wait_ticks;
    int previous_duration;
    uint8_t* track_data_location;
    uint8_t* position;
    struct dse_track_loop loop_stack[4];
    uint32_t field_0x50;
    struct dse_channel* channel;
    uint32_t field_0x58;
};
ASSERT_SIZE(struct dse_track, 92);

struct dse_sequence {
    int8_t initialized;
    int8_t is_stopped;
    int8_t restart;
    uint8_t has_looped;
    int8_t is_playing;
    int8_t field_0x5;
    int8_t field_0x6;
    int8_t field_0x7;
    int8_t field_0x8;
    int8_t field_0x9;
    int8_t field_0xA;
    uint8_t field_0xB;
    uint8_t active_fades_flags;
    uint8_t num_tracks_playing;
    int8_t num_tracks;
    int8_t num_tracks_2;
    int8_t num_channels;
    uint8_t field_0x11;
    uint8_t field_0x12;
    uint8_t global_volume_index;
    uint8_t effect_id;
    uint8_t seq_ticks_per_eighth_beat;
    uint8_t field_0x16;
    uint8_t bpm;
    uint8_t field_0x18;
    uint8_t field_0x19;
    int16_t ticks_until_update;
    void* file;
    int field_0x20;
    struct dse_synth* synth;
    int id;
    uint16_t field_0x2C; // Something to do with sound effects
    uint16_t field_0x30;
    int total_driver_ticks_at_reset;
    int field_0x38;
    int seq_tick_counter;
    int time_since_last_seq_tick;
    int beatsplit_times_ticktime;
    int microseconds_per_beat;
    struct dse_fade tempo;
    struct dse_fade bend;
    struct dse_fade volume;
    struct dse_fade pan;
    uint8_t field_0x88[16];
    int8_t volume_fade_target;
    uint8_t field_0x99[3];

    void* signal_callback;
    void* callback_arg;
    struct dse_sequence* next;

    struct dse_track tracks[];
};
ASSERT_SIZE(struct dse_sequence, 168);

struct dse_voice {
    uint32_t allocated_sequence_id;
    uint16_t note_off_flag;
    uint16_t update_flags;
    uint16_t hw_voice_index;
    uint16_t hw_voice_bit;
    uint8_t field_0xC;
    uint8_t current_note_number;
    int8_t wave_index;
    uint8_t key_bend_sensitivity;
    int16_t key_bend;
    uint16_t note_key;
    int16_t key_base;
    uint8_t note_volume;
    uint8_t wave_pan;
    uint16_t channel_and_keygroup;
    uint16_t field_0x1C;
    struct dse_sample sample;
    struct sound_envelope envelope;
    struct dse_lfo_bank lfo_bank;
    int timer;
    int volume;
    int pan;
    uint32_t field_0x148;
    uint8_t envelope_volume;
    uint8_t field_0x14D[7];
    struct dse_voice* next_in_channel_allocation_list;
    struct dse_channel* channel_allocation;
};
ASSERT_SIZE(struct dse_voice, 348);

struct dse_wave {
    uint8_t field_0x0[16];
    struct dse_sample sample;
};
ASSERT_SIZE(struct dse_wave, 48);

struct dse_wavebank {
    void* file;
    uint16_t id;
    uint16_t mainbank_id;
    int16_t num_waves;
    uint16_t num_instruments;
    uint8_t num_keygroups;
    uint8_t sample_container_kind;
    uint16_t field_0xE;

    void* wavi_chunk_location;
    void* prgi_chunk_location;
    void* kgrp_chunk_location;
    void* pcmd_chunk_location;

    uint32_t pcm_data_size;

    struct dse_wavebank* next;
};
ASSERT_SIZE(struct dse_wavebank, 40);

struct dse_driver_settings {
    uint32_t error_flags;
    uint32_t field_0x4;

    void* heap_location;
    uint32_t heap_size;

    uint8_t field_0xC;
    uint8_t is_tick_rate_60hz;
    uint8_t field_0xE;
    uint8_t field_0xF;
    int16_t field_0x10;
    int16_t field_0x12;
    uint16_t num_se_sequences;
    uint16_t tracks_per_se_sequence;

    struct dse_heap_allocator heap_allocator;

    void* error_callback;
    void* error2_callback;
    uint32_t field_0x30;
};
ASSERT_SIZE(struct dse_driver_settings, 52);

struct dse_driver_work {
    int8_t initialized;
    uint8_t stopped;
    uint16_t unknown_0x2;
    int error_code;
    uint32_t error_data;
    uint32_t error_data2;
    int error2_code;
    uint32_t error2_data;
    uint32_t error2_data2;
    void* error_callback;
    void* error2_callback;
    uint32_t field_0x24;
    int16_t microseconds_per_driver_tick;
    uint16_t field_0x2A;
    int total_driver_ticks;
    int driver_tick_rate;
    uint32_t current_rng_seed;
    uint8_t field_0x38;
    uint8_t field_0x39;
    uint8_t last_se_bank_num_sequences;
    uint8_t last_se_bank_tracks_per_sequence;
    uint8_t field_0x3C;
    int8_t field_0x3D;
    uint16_t field_0x3E;
    uint8_t global_volumes[16];
    struct dse_note_list_node notes[128];
    struct dse_note_list_node* free_notes_list;
    uint32_t field_0x654;
    struct dse_synth* synth_list;
    uint32_t field_0x65C;
    struct dse_wavebank* loaded_wavebanks_list;
    struct dse_sequence* bgm_sequences_list;
    struct dse_sequence* se_sequences_list;
    struct dse_se_bank* loaded_effect_banks;
    uint8_t field_0x670[132];
    struct dse_heap_node* heap_node_list;
    void* heap_end;
    int heap_size;
    struct dse_heap_allocator heap_allocator;
    uint8_t field_0x70C[36];
    int16_t num_voices;
    uint16_t active_voices_bits;
    uint16_t start_voices_bits;
    uint16_t deactivate_voices_bits;
    uint16_t deallocate_voices_bits;
    uint16_t field_0x73C;

    struct dse_voice voices[16];

    uint16_t field_0x1CFC;
    uint16_t field_0x1CFE;
    int num_active_voices;
    int prev_nom_active_voices;
    int ticks_until_num_active_voices_timeout;
    uint32_t microseconds_per_driver_tick_2;
    uint32_t field_1D10;
    uint32_t field_1D14;

    struct thread driver_thread;
    void* driver_stack;

    uint32_t field_0x1DDC;
    struct dse_mainbank* loaded_mainbanks;
    int mainbank_max_read_size;
    void* mainbank_read_callback;
    void* mainbank_read_callback_param;

    uint8_t loader_thread_priority;
    uint8_t field_0x1DF1;
    int8_t quit_sample_loader;
    int8_t is_sample_loader_sleeping;
    struct thread sample_loader_thread;
    void* sample_loader_thread_stack;
    struct dse_wavebank* loading_bank;
    void* loading_bank_pcm_data;
};
ASSERT_SIZE(struct dse_driver_work, 7872); // Likely longer.

#endif
