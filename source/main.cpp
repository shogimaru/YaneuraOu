﻿//#include <iostream>
//#include "bitboard.h"
//#include "position.h"
#include "search.h"
#include "thread.h"
#include "tt.h"
#include "usi.h"
#include "misc.h"

// ファイルの中身を出力する。
void print_file(const std::string& path)
{
	SystemIO::TextReader reader;
	if (reader.Open(path).is_not_ok())
		return;

	std::string line;
	while (reader.ReadLine(line).is_ok())
		sync_cout << line << sync_endl;
}

// ----------------------------------------
//  main()
// ----------------------------------------

//int main(int argc, char* argv[])
int YaneuraOu()
{
	// 起動時に説明書きを出力。
	print_file("startup_info.txt");

	// --- 全体的な初期化

	int argc = 1;
	char* argv[] = { (char*)"." };

	static bool once = [&]() {
	Bitboards::init();
	Position::init();

	USIEngine engine(argc,argv);

	USI::init(Options);
	Search::init();

	Eval::init();
	return true;
	}();

	// エンジンオプションの"Threads"があるとは限らないので…。
	size_t thread_num = Options.count("Threads") ? (size_t)Options["Threads"] : 1;
	Threads.set(thread_num);

//#if !defined(__EMSCRIPTEN__)
#if 1
	// USIコマンドの応答部

	USI::loop(argc, argv);

	// 生成して、待機させていたスレッドの停止

	Threads.set(0);
#else
	// yaneuraOu.wasm
	// ここでループしてしまうと、ブラウザのメインスレッドがブロックされてしまうため、コメントアウト
#endif

	return 0;
}
