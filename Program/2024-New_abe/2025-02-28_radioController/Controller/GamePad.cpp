#include"GamePad.hpp"


void DrawGamepadScreen(int num, int input, Stick L, Stick R){
	int x = 0;
	int y = 0;
	int w = 0;
	int h = 0;
	int r = 150;
	int ofs = r * 3;


	// button

	// back
	y = CENTER_Y - ofs;
	h = 50;

	// L
	if (input & BUTTON_L) {
		DrawBoxAA(CENTER_X - ofs - r, y, CENTER_X - ofs + r, y + h, dark_red, TRUE);
	}
	DrawBoxAA(CENTER_X - ofs - r, y, CENTER_X - ofs + r, y + h, white, FALSE);
	DrawString(CENTER_X - ofs - GetDrawStringWidth("L", strlen("L")) / 2, y + h / 2 - font_size / 2, "L", white);

	// R
	if (input & BUTTON_R) {
		DrawBoxAA(CENTER_X + ofs - r, y, CENTER_X + ofs + r, y + h, dark_red, TRUE);
	}
	DrawBoxAA(CENTER_X + ofs - r, y, CENTER_X + ofs + r, y + h, white, FALSE);
	DrawString(CENTER_X + ofs - GetDrawStringWidth("R", strlen("R")) / 2, y + h / 2 - font_size / 2, "R", white);


	// ABXY
	x = CENTER_X + ofs;
	y = CENTER_Y - r - 40;
	w = 40;
	r = 100;

	// A
	if (input & BUTTON_A) {
		DrawCircleAA(x + r, y, w, 96, dark_red, TRUE);
	}
	DrawCircleAA(x + r, y, w, 96, white, FALSE);
	DrawString(x + r - GetDrawStringWidth("A", strlen("A")) / 2, y - font_size / 2, "A", white);

	// Y
	if (input & BUTTON_Y) {
		DrawCircleAA(x - r, y, w, 96, dark_red, TRUE);
	}
	DrawCircleAA(x - r, y, w, 96, white, FALSE);
	DrawString(x - r - GetDrawStringWidth("Y", strlen("Y")) / 2, y - font_size / 2, "Y", white);


	// B
	if (input & BUTTON_B) {
		DrawCircleAA(x, y + r, w, 96, dark_red, TRUE);
	}
	DrawCircleAA(x, y + r, w, 96, white, FALSE);
	DrawString(x - GetDrawStringWidth("B", strlen("B")) / 2, y + r - font_size / 2, "B", white);

	// X
	if (input & BUTTON_X) {
		DrawCircleAA(x, y - r, w, 96, dark_red, TRUE);
	}
	DrawCircleAA(x, y - r, w, 96, white, FALSE);
	DrawString(x - GetDrawStringWidth("X", strlen("X")) / 2, y - r - font_size / 2, "X", white);


	// CROSS
	r = 150;
	x = CENTER_X - ofs;
	y = CENTER_Y + r + 40;
	w = 40;
	r = 100;

	if (L.x == 0 && L.y == 0) {
		// A
		if (input & BUTTON_RIGHT) {
			DrawCircleAA(x + r, y, w, 96, dark_red, TRUE);
		}
		// Y
		if (input & BUTTON_LEFT) {
			DrawCircleAA(x - r, y, w, 96, dark_red, TRUE);
		}
		// B
		if (input & BUTTON_DOWN) {
			DrawCircleAA(x, y + r, w, 96, dark_red, TRUE);
		}
		// X
		if (input & BUTTON_UP) {
			DrawCircleAA(x, y - r, w, 96, dark_red, TRUE);
		}
	}

	// A
	DrawCircleAA(x + r, y, w, 96, white, FALSE);
	DrawString(x + r - GetDrawStringWidth("¨", strlen("¨")) / 2, y - font_size / 2, "¨", white);

	// Y
	DrawCircleAA(x - r, y, w, 96, white, FALSE);
	DrawString(x - r - GetDrawStringWidth("©", strlen("©")) / 2, y - font_size / 2, "©", white);


	// B
	DrawCircleAA(x, y + r, w, 96, white, FALSE);
	DrawString(x - GetDrawStringWidth("«", strlen("«")) / 2, y + r - font_size / 2, "«", white);

	// X
	DrawCircleAA(x, y - r, w, 96, white, FALSE);
	DrawString(x - GetDrawStringWidth("ª", strlen("ª")) / 2, y - r - font_size / 2, "ª", white);


	// PLUS MINUS
	x = CENTER_X - ofs + r * 3;
	y = CENTER_Y - r - 40;
	w = 40;

	// MINUS
	if (input & BUTTON_MINUS) {
		DrawCircleAA(x, y, w, 96, dark_red);
	}
	DrawCircleAA(x, y, w, 96, white, FALSE);
	DrawString(x - GetDrawStringWidth("[", strlen("[")) / 2, y - font_size / 2, "[", white);

	// PLUS
	x = CENTER_X + ofs - r * 3;
	if (input & BUTTON_PLUS) {
		DrawCircleAA(x, y, w, 96, dark_red);
	}
	DrawCircleAA(x, y, w, 96, white, FALSE);
	DrawString(x - GetDrawStringWidth("{", strlen("{")) / 2, y - font_size / 2, "{", white);



	// stick
	// left
	r = 150;
	x = CENTER_X - ofs;
	y = CENTER_Y - r - 40;

	if (input & BUTTON_L_STICK) {
		DrawCircleAA(x, y, r, 96, dark_red, TRUE);
	}
	DrawCircleAA(x, y, r, 96, white, FALSE);
	DrawLineAA(x - r, y, x + r, y, white);
	DrawLineAA(x, y - r, x, y + r, white);

	L.rad = atan2((double)L.y, (double)L.x);
	L.len = sqrt(L.x * L.x + L.y * L.y);
	if (L.len > 1000) L.len = 1000;
	DrawCircleAA(x + cos(L.rad) * L.len * r / 1000, y + sin(L.rad) * L.len * r / 1000, 32, 96, red, TRUE);


	// right
	x = CENTER_X + ofs;
	y = CENTER_Y + r + 40;

	if (input & BUTTON_R_STICK) {
		DrawCircleAA(x, y, r, 96, dark_red, TRUE);
	}
	DrawCircleAA(x, y, r, 96, white, FALSE);
	DrawLineAA(x - r, y, x + r, y, white);
	DrawLineAA(x, y - r, x, y + r, white);

	R.rad = atan2((double)R.y, (double)R.x);
	R.len = sqrt(R.x * R.x + R.y * R.y);
	if (R.len > 1000) R.len = 1000;
	DrawCircleAA(x + cos(R.rad) * R.len * r / 1000, y + sin(R.rad) * R.len * r / 1000, 32, 96, red, TRUE);



	// debug
	char input_str[8 * sizeof(input) + 1] = "";
	for (int i = 0; i < 8 * sizeof(input); i++) {
		if (input >> (8 * sizeof(input) - i - 1) & 1) {
			input_str[i] = '1';
		}
		else {
			input_str[i] = '0';
		}
	}


	DrawFormatString(8, 8, white,
		"Num:%d\nInput:%s\n%d\n\nL\n x:%d\n y:%d\n deg:%lf\n len:%lf\n\nR\n x:%d\n y:%d\n",
		num, input_str, input, L.x, L.y, L.rad * 180 / PI, L.len, R.x, R.y);

}