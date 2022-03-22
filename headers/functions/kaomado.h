// All Kaomado loads use the same stream, so it's initialized once on game start!
void InitStreamKaomado(void);

void InitKaomado(struct kaomado*);
void InitKaomadoSpecies(struct kaomado*, int16_t species_id);
void SetKaomadoEmote(struct kaomado*, uint8_t emote_idx);
void SetKaomadoLayout(struct kaomado*, uint8_t layout_idx);
void SetKaomadoOffset(struct kaomado*, struct portrait_offset* offset);
void AllowKaomadoDefault(struct kaomado*, bool allow);
void IsValidKaomado(struct kaomado*);
bool LoadKaomado(struct kaomado*, void* buffer);
