#ifndef MAIN_H
#define MAIN_H

enum SfxType {
	S_START,
	S_JUMP,
	S_MATCH,
	MAXSFX
};

void playSound( SfxType id);

#endif
