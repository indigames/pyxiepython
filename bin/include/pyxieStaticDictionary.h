///////////////////////////////////////////////////////////////
//Pyxie game engine
//
//  Copyright Kiharu Shishikura 2019. All rights reserved.
///////////////////////////////////////////////////////////////
#pragma once
#include "pyxieObject.h"

namespace pyxie
{
	struct PatriciaPackedNode
	{
		int16_t bit;
		uint16_t entry;
		uint16_t left;
		uint16_t right;
		uint32_t nameEntry;
	};

	class PYXIE_EXPORT pyxieStaticDictionary : public pyxieObject
	{
		PatriciaPackedNode * packedhead;
		char* packedkeys;
	protected:
		uint32_t node_cnt;

		PatriciaPackedNode* LookupPackedNode(const char* k);
		uint32_t bit_get(const char* bit_stream, int32_t n);
		bool key_compare(const char* k1, const char* k2);
	public:
		pyxieStaticDictionary(PatriciaPackedNode* nodes, char* keys, uint32_t numNodes);
		pyxieStaticDictionary();
		~pyxieStaticDictionary() {}
		virtual int	Lookup(const char* name);
		virtual int Scan(const char* name, int& pos);

		void Dump();
	};
}