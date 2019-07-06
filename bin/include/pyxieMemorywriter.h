///////////////////////////////////////////////////////////////
//Pyxie game engine
//
//  Copyright Kiharu Shishikura 2019. All rights reserved.
///////////////////////////////////////////////////////////////
#pragma once
#include "pyxieTypes.h"
#include "pyxieUtilities.h"
#include "pyxieMemostream.h"
#include <list>
#include <map>
#include <string>

namespace pyxie {

	class pyxieMemoryWriter{
	public:
		pyxieMemoryWriter(pyxieMemostream* pStream) {
			m_pStream = pStream;
			m_bigEndian = false;
			m_streamOwner = false;
		}

		~pyxieMemoryWriter() {
			if (m_pStream && m_streamOwner) {
				delete m_pStream;	// destructor will close the file too
			}
		}
		void    Close(void) {
			PatchOffsets();
			if (m_pStream && m_streamOwner) {
				delete m_pStream;	// destructor will close the file too
				m_pStream = 0;
			}
		}
		void    Align(unsigned int alignment) {
			long posCurrent = GetPos();
			long posNext = ALIGN(posCurrent, alignment);
			for (long pos = posCurrent; pos < posNext; pos++) {
				static const unsigned char zero = 0;
				m_pStream->write(reinterpret_cast<const char*>(&zero), 1);
			}
		}
		void    Label(const std::string& label, unsigned int alignment = 1) {
			long posCurrent = GetPos();
			auto val = ALIGN(posCurrent, alignment);
			m_labels[label] = (long)val;
		}
		void    WriteOffset16(const std::string& label, const std::string& labelRelativeTo = std::string("")) {
			// Write placeholder 0 - will stay 0 if label is unresolved
			AddOffset(OffsetDef::kOfs16, label, labelRelativeTo);
			Write((unsigned short)0);
		}
		void    WriteOffset32(const std::string& label, const std::string& labelRelativeTo = std::string("")) {
			// Write placeholder 0 - will stay 0 if label is unresolved
			AddOffset(OffsetDef::kOfs32, label, labelRelativeTo);
			Write((unsigned int)0);
		}
		void    PatchOffsets(void) {
			long posSaved = GetPos();

			// Patch all offsets
			for (std::list<OffsetDef>::const_iterator iteOfs = m_offsets.begin();
				iteOfs != m_offsets.end();
				iteOfs++) {
				// Resolve label
				std::map<std::string, long>::const_iterator foundLabel = m_labels.find(iteOfs->m_label);
				if (foundLabel != m_labels.end()) {
					long posRelativeTo = iteOfs->m_posRelativeTo;

					// Resolve base label, if specified
					if (iteOfs->m_posRelativeTo) {
						std::map<std::string, long>::const_iterator foundLabelRelative = m_labels.find(iteOfs->m_labelRelativeTo);
						if (foundLabelRelative != m_labels.end())
							posRelativeTo = foundLabelRelative->second;
					}
					if (posRelativeTo >= 0) {
						long ofs = foundLabel->second - posRelativeTo;
						m_pStream->seekp(iteOfs->m_posOffset);
						switch (iteOfs->m_type) {
						case OffsetDef::kOfs16:
							Write((unsigned short)ofs);
							break;
						case OffsetDef::kOfs32:
							Write((unsigned int)ofs);
							break;
						default:
							break;
						}
					}
				}
			}

			// Remove offsets / restore file pos
			m_offsets.clear();
			m_pStream->seekp(posSaved);
		}
		long    GetPos(void) {
			return (long)m_pStream->getSize();
		}
		long    GetLabelOffset(const std::string& label) {
			std::map<std::string, long>::const_iterator foundLabel = m_labels.find(label);

			if (foundLabel != m_labels.end())
				return foundLabel->second;
			else
				return -1;
		}

		template<typename T>
		void Write(T val);

	private:
		struct OffsetDef
		{
			enum Type
			{
				kOfs16,
				kOfs32
			}               m_type;
			std::string     m_label;
			std::string     m_labelRelativeTo;
			long            m_posOffset;
			long            m_posRelativeTo;
		};
		void    AddOffset(OffsetDef::Type type, const std::string& label, const std::string& labelRelativeTo) {
			OffsetDef ofsDef;
			ofsDef.m_type = type;
			ofsDef.m_label = label;
			ofsDef.m_labelRelativeTo = labelRelativeTo;

			// Current position
			long posCurrent = GetPos();
			ofsDef.m_posOffset = posCurrent;

			// If position is not specified, it's relative to current
			if (labelRelativeTo == "") {
				ofsDef.m_posRelativeTo = posCurrent;
			}
			else {
				ofsDef.m_posRelativeTo = -1;
			}

			// Add offset to the list
			m_offsets.push_back(ofsDef);
		}

		pyxieMemostream*				m_pStream;
		std::list<OffsetDef>            m_offsets;
		std::map<std::string, long>     m_labels;
		bool                            m_bigEndian;
		bool							m_streamOwner;
	};

	template<typename T>
	inline void pyxieMemoryWriter::Write(T val)
	{
		m_pStream->write(reinterpret_cast<const char*>(&val), sizeof(T));
	}

}
