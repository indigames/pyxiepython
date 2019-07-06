#pragma once

#include "pyxieTypes.h"

namespace pyxie
{
	class PYXIE_EXPORT pyxieFigureExportConfigManager {
		bool	TriangleStrip;				//Convert vertices to triangle strip format
		bool	FreezeGeometoryTransform;	//ジオメトリのトランスフォームをフリーズする
		bool	FullPathName;				//ノード名をルートからの親の名前を'/'でつなげた名前に変更する
		bool	OutputNameList;				//jointsとmaterialのノード名を出力する
		bool	OutputNotes;				//jointsとmaterialのノード名を出力する
		bool	GenMipmap;					//テクスチャのミップマップを生成する
		bool	ClopTransform;				//スキンウェイトを持ったオブジェクトノードのトランスフォームは削除する
		bool	ComputePeriod;
		float	BaseScale;
		float	Tolerance;
		pyxieFigureExportConfigManager();
	public:
		static pyxieFigureExportConfigManager& Instance();

		void ReadConfigFiles(const char* filePath);

		bool	IsTriangleStrip() { return TriangleStrip; }
		bool	IsFreezeGeometoryTransform() { return FreezeGeometoryTransform; }
		bool	IsFullPathName() { return FullPathName; }
		bool	IsOutputNameList() { return OutputNameList; }
		bool	IsOutputNotes() { return OutputNotes; }
		bool	IsGenMipmap() { return GenMipmap; }
		bool	IsClopTransform() { return ClopTransform; }
		bool	IsComputePeriod() { return ComputePeriod; }
		float	GetBaseScale() { return BaseScale; }
		float	GetTolerance() { return Tolerance; }
		void	SetBaseScale(float v) { BaseScale = v; }
	};
}
