///////////////////////////////////////////////////////////////
//Pyxie game engine
//
//  Copyright Kiharu Shishikura 2019. All rights reserved.
///////////////////////////////////////////////////////////////
#pragma once

#include "pyxieMathutil.h"
#include <vector>
#include <set>

namespace pyxie
{
	class pyxieBinarySpaceTree
	{
		struct BinarySpaceTreeItem
		{
			pyxieAABBox box;
			int item;
			int numVertices;
		};

		class BinarySpaceTreeNode
		{
			pyxieAABBox box;
			static const int numChildren = 4;
			static const int MinimumSplitVertices = 200;
			BinarySpaceTreeNode* children[numChildren];
			std::vector<BinarySpaceTreeItem*> items;
		public:
			BinarySpaceTreeNode(const pyxieAABBox& box)
			{
				this->box = box;
				for (int i = 0; i < numChildren; i++) children[i] = NULL;
			}

			~BinarySpaceTreeNode()
			{
				for (std::vector<BinarySpaceTreeItem*>::iterator it = items.begin(); it != items.end(); it++) delete(*it);

				for (int i = 0; i < numChildren; i++)
					if (children[i]) delete children[i];
			}

			int NumItems()
			{
				return static_cast<int>(items.size());
			}

			void CalcTotalItems(int& num)
			{
				num += NumItems();
				for (int i = 0; i < numChildren; i++)
				{
					if (children[i]) children[i]->CalcTotalItems(num);
				}
			}

			void AddItems(std::vector<BinarySpaceTreeItem*>& parentItems)
			{
				std::vector<BinarySpaceTreeItem*>::iterator it = parentItems.begin();
				while (it != parentItems.end())
				{
					if (AddItem(*it))
						it = parentItems.erase(it);
					else
						++it;
				}

				uint32_t numVetrices = 0;
				it = items.begin();
				while (it != items.end())
				{
					numVetrices += (*it)->numVertices;
					++it;
				}

				if (numVetrices > MinimumSplitVertices)
				{
					pyxieAABBox childBox[4];
					SplitBox(childBox[0], childBox[1], childBox[2], childBox[3]);

					for (int i = 0; i < numChildren; i++)
					{
						if (items.size() == 0) break;

						children[i] = new BinarySpaceTreeNode(childBox[i]);
						children[i]->AddItems(items);
						int totalItems = 0;
						children[i]->CalcTotalItems(totalItems);
						if (totalItems == 0)
						{
							delete children[i]; children[i] = NULL;
						}
					}
				}

				box.MinEdge = Vec3(10000000.0f, 10000000.0f, 10000000.0f);
				box.MaxEdge = Vec3(-10000000.0f, -10000000.0f, -10000000.0f);
				for (int i = 0; i < numChildren; i++)
				{
					if (children[i])
						box.addInternalBox(children[i]->box);
				}
				for (std::vector<BinarySpaceTreeItem*>::iterator it = items.begin(); it != items.end(); it++)
				{
					box.addInternalBox((*it)->box);
				}
			}

			bool AddItem(BinarySpaceTreeItem* item)
			{
				if (item->box.IsFullInside(box))
				{
					items.push_back(item);
					return true;
				}
				return false;
			}


			void GetItems(const pyxieViewFrustum& frustum, std::set<int>& collection, int &numTests)
			{
				numTests++;
				//int ret = frustum.BoxInFrustum(box);
				//if(ret == ViewFrustum::OUTSIDE) return;

				for (int i = 0; i != 6; ++i)
				{
					IntersectionRelation r = box.ClassifyPlaneRelation(frustum.planes[i]);
					if (r == ISREL_BACK)
						return;
				}

				for (std::vector<BinarySpaceTreeItem*>::iterator it = items.begin(); it != items.end(); it++)
					collection.insert((*it)->item);

				for (int i = 0; i < numChildren; ++i)
					if (children[i]) children[i]->GetItems(frustum, collection, numTests);
			}

		private:
			void SplitBox(pyxieAABBox& left, pyxieAABBox& right, pyxieAABBox& top, pyxieAABBox& bottom)
			{
				Vec3 s = (box.MaxEdge - box.MinEdge)*0.5f;

				if (s.X() > 0.01f)
				{
					left.MinEdge = box.MinEdge;
					left.MaxEdge = box.MaxEdge;
					left.MaxEdge.X(box.MinEdge.X() + s.X());

					right.MinEdge = box.MinEdge;
					right.MaxEdge = box.MaxEdge;
					right.MinEdge.X(box.MinEdge.X() + s.X());
				}
				else
				{
					left.MinEdge = Vec3(0, 0, 0);
					left.MaxEdge = Vec3(0, 0, 0);
					right.MinEdge = Vec3(0, 0, 0);
					right.MaxEdge = Vec3(0, 0, 0);
				}

				if (s.Z() > 0.01f)
				{
					top.MinEdge = box.MinEdge;
					top.MaxEdge = box.MaxEdge;
					top.MaxEdge.Z(box.MinEdge.Z() + s.Z());

					bottom.MinEdge = box.MinEdge;
					bottom.MaxEdge = box.MaxEdge;
					bottom.MinEdge.Z(box.MinEdge.Z() + s.Z());
				}
				else
				{
					top.MinEdge = Vec3(0, 0, 0);
					top.MaxEdge = Vec3(0, 0, 0);
					bottom.MinEdge = Vec3(0, 0, 0);
					bottom.MaxEdge = Vec3(0, 0, 0);
				}
			}
			friend class BinaryTreeRenderer;
		};

		BinarySpaceTreeNode* root;
		std::vector<BinarySpaceTreeItem*> items;
		int numTests;

	public:
		pyxieBinarySpaceTree()
		{
			numTests = 0;
			root = NULL;
		}

		~pyxieBinarySpaceTree()
		{
			if (root)delete root;
		}

		inline int GetClippingTestCount() { return numTests; }

		void AddRootItem(int param, const pyxieAABBox& box, uint32_t numVert)
		{
			BinarySpaceTreeItem* item = new BinarySpaceTreeItem;
			item->box = box;
			item->item = param;
			item->numVertices = numVert;
			items.push_back(item);
		}

		void BuildTree(const pyxieAABBox& box)
		{
			root = new BinarySpaceTreeNode(box);
			root->AddItems(items);
		}

		void Clip(const pyxieViewFrustum& frustum, std::set<int>& items)
		{
			if (!root) return;
			numTests = 0;


			for (std::vector<BinarySpaceTreeItem*>::iterator it = this->items.begin(); it != this->items.end(); it++)
				items.insert((*it)->item);

			root->GetItems(frustum, items, numTests);
		}

		void ClipAsync(const pyxieViewFrustum& frustum, std::set<int>& items);

		void ClipWait();


		friend class pyxieBinaryTreeRenderer;
	};
}