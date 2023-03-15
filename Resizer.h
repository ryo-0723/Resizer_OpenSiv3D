#pragma once
#include <Siv3D.hpp>
#include "ScreenStyle.h"


class Resizer {
private:
	Vec2 Virtual_Size;
	Vec2 Scall;
	ScreenStyle Style;
	Vec2 Offset_Pos;
	Vec2 Scalling;
	Vec2 FullScreen_Size;
public:
	/// @brief 画面サイズの変更に応じて座標の自動変換をするクラス
	/// @param Virtual_Size 変換先の画面サイズ
	/// @param Center リサイズの画面タイプ
	/// @param FullScreen_Size Keepの時に使用する最大ウィンドウサイズ
	Resizer(Vec2 Virtual_Size ,ScreenStyle Style = ScreenStyle::Center, Vec2 FullScreen_Size={700,700})
		:Virtual_Size(Virtual_Size)
		,Style(Style)
		,Scall()
		,Offset_Pos()
		,Scalling()
		,FullScreen_Size(FullScreen_Size)
	{
		Window::SetStyle(WindowStyle::Sizable);
		Window::Maximize();
	}

	/// @brief リサイズを実行する関数
	void Update(const RectF& Screen) {
		//基にする画面とそろえる画面との倍率決める
		Scalling = Screen.size / Virtual_Size;
		switch (Style) {
		case ScreenStyle::Center:
			Scall.x = Scall.y = Scalling.x < Scalling.y ? Scalling.x : Scalling.y;
			Offset_Pos = Screen.pos + (Screen.size - Virtual_Size * Scall) * 0.5;
			break;
		case ScreenStyle::Left:
			Scall.x = Scall.y = Scalling.x < Scalling.y ? Scalling.x : Scalling.y;
			//x,yのうち、より縮小しなければ画面に入らないほうの縮小率を求める
			Offset_Pos.x = Screen.pos.x;
			Offset_Pos.y = Screen.pos.y + (Screen.size.y - Virtual_Size.y * Scall.y) * 0.5;
			//左上の座標原点を調整する
			break;
		case ScreenStyle::Right:
			Scall.x = Scall.y = Scalling.x < Scalling.y ? Scalling.x : Scalling.y;	
			Offset_Pos.x = Screen.pos.x + (Screen.size.x - Virtual_Size.x * Scall.x);
			Offset_Pos.y = Screen.pos.y + (Screen.size.y - Virtual_Size.y * Scall.y) * 0.5;
			break;
		case  ScreenStyle::Keep:
			Offset_Pos = Screen.pos;
			Scall = FullScreen_Size / Virtual_Size;
			if (Screen.size.x >= FullScreen_Size.x && Screen.size.y >= FullScreen_Size.y){
				//フルスクリーンの時は画面移動のバーを消した座標系を作る
				
			}
			else {
				//画面移動用のバーを描く
				//バー分だけ縮めた座標系を作る
				RectF{ Screen.bl().x,Screen.bl().y - 30,Screen.size.x,30}.draw(Palette::Aqua);
				//バー分だけ縮めた座標系を作る
				RectF{ Screen.tr().x-30,Screen.tr().y,30,Screen.size.y}.draw(Palette::Aqua);
			}
			break;
		}
	}

	/// @brief 実ウィンドウでの座標になるよう計算
	/// @param Pos 仮想ウィンドウでの座標
	Vec2  Cal_Pos(const Vec2& Pos) const {
		return Pos * Scall + Offset_Pos;
	}
	/// @brief 実ウィンドウでの左下からの座標になるように計算
	/// @param Pos 仮想ウィンドウでの座標
	/// @return アプリ内での座標
	Vec2 Pos_bottom(const Vec2& Pos) const{
		return { Pos.x * Scall.x + Offset_Pos.x, (Virtual_Size.y * Scall.y - Pos.y * Scall.y)+Offset_Pos.y };
	}

	/// @brief 実ウィンドウでの大きさになるように計算
	/// @param Size 仮想ウィンドウでの大きさ
	Vec2 Cal_Size(const Vec2& Size) const {
		return  Size * Scall;
	}

	/// @brief 実ウィンドウでの大きさになるように計算
	/// @param Size 仮想ウィンドウでの大きさ
	double Cal_SizeX(const double& Size)const {
		return Size * Scall.x;
	}

	/// @brief 実ウィンドウでの大きさになるように計算
    /// @param Size 仮想ウィンドウでの大きさ
	double Cal_SizeY(const double& Size)const {
		return Size * Scall.y;
	}

	Line toReal(const Line& Raw) const {
		return Line{ Cal_Pos(Raw.begin),Cal_Pos(Raw.end) };
	}

	RectF toReal(const RectF& Raw) const {
		return RectF{ Cal_Pos(Raw.pos),Cal_Size(Raw.size) };
	}

	RoundRect toReal(const RoundRect& Raw) const {
		return RoundRect{ Cal_Pos(Raw.rect.pos),Cal_Size(Raw.rect.size),Cal_SizeX(Raw.r) };
	}

	Circle toReal(const Circle& Raw) const {
		return Circle{ Cal_Pos(Raw.center),Cal_SizeX(Raw.r) };
	}

	Triangle toReal(const Triangle& Raw)const {
		return Triangle{ Cal_Pos(Raw.p0),Cal_Pos(Raw.p1),Cal_Pos(Raw.p2) };
	}

	Bezier2 toReal(const Bezier2& Raw)const {
		return Bezier2{ Cal_Pos(Raw.p0),Cal_Pos(Raw.p1),Cal_Pos(Raw.p2) };
	}

	Bezier3 toReal(const Bezier3& Raw)const {
		return Bezier3{ Cal_Pos(Raw.p0),Cal_Pos(Raw.p1),Cal_Pos(Raw.p2),Cal_Pos(Raw.p3) };
	}


};
