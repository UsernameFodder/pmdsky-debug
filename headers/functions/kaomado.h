// All Kaomado loads use the same stream, so it's initialized once on game start!
void InitKaomadoStream(void);

void InitKaomado(struct kaomado*);
void InitKaomadoSpecies(struct kaomado*, int16_t species_id);
void SetKaomadoEmote(struct kaomado*, uint8_t emote_idx);
void SetKaomadoLayout(struct kaomado*, uint8_t layout_idx);
void SetKaomadoOffset(struct kaomado*, struct portrait_offset* offset);
void AllowKaomadoDefault(struct kaomado*, bool allow);
void IsValidKaomado(struct kaomado*);
bool LoadKaomado(struct kaomado*, void* buffer); // This function updates all three of the flip fields!

uint8_t CreateKaomadoCanvas(enum screen, uint32_t palette_idx, bool is_framed);
void FreeKaomadoCanvas(uint8_t canvas); // Disables the canvas and frees resources
bool CheckKaomadoCanvasBusy(uint8_t canvas); // Busy means the canvas is updating in any way
void CommitKaomadoCanvas(uint8_t canvas, struct kaomado*); // Writes the passed Kaomado to the canvas buffer
void HideKaomadoCanvas(uint8_t canvas);
void UpdateKaomadoCanvas(uint8_t canvas); // Not to be called manually