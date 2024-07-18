#include <Novice.h>
#include <Novice.h>
#include <imgui.h>
#include "MakeMatrix.h"
#include "MatrixMath.h"
#include "Vector3Math.h"
#include "Draw.h"
#include "algorithm"

const char kWindowTitle[] = "LE2B_17_ナガイ_コハク_MT3_4_1 等速円運動";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	//ウィンドウサイズ
	float kWindowWidth = 1280.0f;
	float kWindowHeight = 720.0f;

	//カメラ:平行移動
	Vector3 cameraTranslate{ 0.0f,1.9f,-6.49f };

	//カメラ:回転
	Vector3 cameraRotate{ 0.26f,0.0f,0.0f };

	//カメラ:距離
	Vector3 baseOffset{ 0.0f,0.0f,-6.0f };

	//円運動の中心座標
	Vector3 center{ 0.0f,0.0f,0.0f };

	//円運動後の座標
	Vector3 point{ 0.0f,0.0f,0.0f };

	//円運動の半径
	float radius = 0.8f;

	//角加速度
	float angleVelocity = 3.14f;

	//角度
	float angle = 0.0f;

	//スタートフラグ
	bool startMove = false;

	//描画クラス
	Draw draw;

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		float deltaTime = 1.0f / 60.0f;

		if (startMove) {

			//角度の計算
			angle += angleVelocity * deltaTime;
		}

		//等速円運動の計算
		point.x = center.x + std::cosf(angle) * radius;
		point.y = center.y + std::sinf(angle) * radius;
		point.z = center.z;

		ImGui::Text("WASD : CameraRotate");
		ImGui::Text("UpArrow : ZoomIn");
		ImGui::Text("DownArrow : ZoomDown");

		ImGui::Text("");

		if (ImGui::TreeNode("Camera")) {
			ImGui::DragFloat3("Translate", &cameraTranslate.x, 0.01f);
			ImGui::DragFloat3("Rotate", &cameraRotate.x, 0.01f);
			ImGui::DragFloat3("offset", &baseOffset.x, 0.01f);
			ImGui::TreePop();
		}

		ImGui::Text("");

		if (ImGui::Button("StartMove")) {

			//ボタンが押されたらフラグをtrueにする
			startMove = true;
		}

		draw.CamaraRotate(baseOffset, cameraRotate, cameraTranslate, keys);

		draw.Pipeline(cameraTranslate, cameraRotate, kWindowWidth, kWindowHeight);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		draw.DrawGrid();

		draw.DrawSphere(Sphere(point, 0.1f), BLUE);

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
