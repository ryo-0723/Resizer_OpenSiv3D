#pragma once


enum class ScreenStyle :uint8
{
	Left,
	//画面を左に張り付けて、アスペクト比を一定のままリサイズする

	Center,
	//画面を中央にして、アスペクト比を一定にする

	Right,
	//画面を右に張り付けて、アスペクト比を一定にする

	Keep,
	//x軸のサイズは固定で、y軸は可変長、アスペクト比は非一定
};
