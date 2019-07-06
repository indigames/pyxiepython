#pragma once
#include <vector>
#include "pyxieDebug.h"

namespace pyxie
{
	class pyxieBinaryFileHelper
	{
	private:
		void*		m_container;				// the allocated memory containing the file data
		uint32_t	m_size;						// (used) size of the data
		uint32_t	m_allocatedSize;			// size of the allocation

	public:
		// constructor
		pyxieBinaryFileHelper()
		{
			m_size = 0;
			m_allocatedSize = 0x1000000;
			m_container = malloc(m_allocatedSize);
		}

		// destructor
		~pyxieBinaryFileHelper()
		{
			free(m_container);
		}

		void Reset()
		{
			m_size = 0;
		}

		// allocate a block
		// NB the returned pointer is only valid until the next call to Allocate()
		void*	Allocate(uint32_t allocationSize, uint32_t alignment, uint32_t& offset)
		{
			// must be power of 2
			pyxie_assert((alignment & (alignment - 1)) == 0);

			// position of new object
			uint32_t pos = m_size;
			if (alignment)
				pos = (pos + alignment - 1) & ~(alignment - 1);

			// do we have space?
			if (pos + allocationSize >= m_allocatedSize)
			{
				uint32_t extra = allocationSize + 0x10000;
				m_allocatedSize += extra;
				m_container = realloc(m_container, m_allocatedSize);
			}

			// zero any pad bytes
			if (pos > m_size)
				memset(GetPointer(m_size), 0, pos - m_size);

			// increment the used size
			m_size = pos + allocationSize;

			offset = pos;
			return (void*)((char*)m_container + pos);
		}

		// return a pointer to data within the container
		// NB the returned pointer is only valid until the next call to Allocate()
		void*	GetPointer(uint32_t offset)
		{
			return (void*)((char*)m_container + offset);
		}

		// get the data size
		uint32_t GetSize() const
		{
			return m_size;
		}
	};



}