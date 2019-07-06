///////////////////////////////////////////////////////////////
//Pyxie game engine
//
//  Copyright Kiharu Shishikura 2019. All rights reserved.
///////////////////////////////////////////////////////////////
#pragma once

#include "pyxieStaticDictionary.h"

#define PATRICIA_KEY_SIZE 64

namespace pyxie
{
	/**@brief パトリシアトライ木を作成し、高速検索 */
	class PYXIE_EXPORT pyxieDictionary : public pyxieStaticDictionary
	{
		struct PatriciaNode
		{
			int32_t bit;
			char *key;
			int32_t data;
			PatriciaNode* left;
			PatriciaNode* right;
		};
		PatriciaNode* head;

	public:
		pyxieDictionary();
		pyxieDictionary(PatriciaPackedNode* nodes, char* keys, uint32_t numNodes);
		~pyxieDictionary();

		bool Insert(const char* k, int32_t d);
		bool Delete(const char* k);
		bool DeleteByData(int32_t data);
		int32_t	Lookup(const char* name);

		bool Load(PatriciaPackedNode* nodes, char* keys, uint32_t numNodes);
		uint32_t Save(PatriciaPackedNode*& nodes, char*& keys, uint32_t& keySize);
	private:
		PatriciaNode * NewNode();
		void InitNode(PatriciaNode* node, const char* k, int32_t d, int32_t bi, PatriciaNode* l, PatriciaNode* r);

		PatriciaNode* LookupNode(const char* k);
		PatriciaNode* LookupNodeRecursive(const char* k, const PatriciaNode* head, const PatriciaNode* curnode = NULL);
		bool LookDown(const char* key, PatriciaNode** prev, PatriciaNode** cur);

		void recursive_remove(PatriciaNode* root);
		bool DeleteNodeByData(int32_t data, PatriciaNode* node);

		void ClearAll();
		void UnpackNode(const PatriciaPackedNode* buff, PatriciaNode* node, uint32_t buff_index, char* pkeys, const PatriciaNode* head);
		void PackNode(const PatriciaNode* node, PatriciaPackedNode* buff, uint16_t& buff_index, char* pkeys, uint32_t& keys_index);

		char* KeyDup(const char* chr);
		void key_copy(PatriciaNode* src, PatriciaNode* dest);
		int32_t bit_first_different(const char* k1, const char* k2);
	};
}