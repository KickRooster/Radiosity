#pragma once
#include <BaseInclude.h>
#include <../3rdParty/cereal/types/memory.hpp>
#include <../3rdParty/cereal/archives/xml.hpp>
#include <fstream>

namespace Core
{
	class FileOperator
	{
	public:
		FileOperator();
		static void * MapMemory(const ANSICHAR * fileFullPathName, Bool binaryFile = False);
		static void UnmapMemory(void * pBytes);

		//	???§Ý?,§Õ??
		template <class T>
		static void SerializeXML(const ANSICHAR * fullPathName, T * t)
		{
			std::ofstream os(fullPathName, std::ios::out);
			cereal::XMLOutputArchive archive(os);

			archive(*t);
		}

		//	???§Ý?,§Õ??[????1????????]
		template <class T>
		static void SerializeXML(const ANSICHAR * fullPathName, T t)
		{
			std::ofstream os(fullPathName, std::ios::out);
			cereal::XMLOutputArchive archive(os);

			archive(*t.get());
		}

		//	?????§Ý?,???
		template <class T>
		static void DeserializeXML(const ANSICHAR * fullPathName, T * t)
		{
			std::ifstream is(fullPathName, std::ios::in);

			if (is.is_open())
			{
				cereal::XMLInputArchive inArchive(is);

				inArchive(*t);

				t->fullPathName = fullPathName;
			}
		}

		//	?????§Ý?,???[????1????????]
		template <class T>
		static void DeserializeXML(const ANSICHAR * fullPathName, T t)
		{
			std::ifstream is(fullPathName, std::ios::in);

			if (is.is_open())
			{
				cereal::XMLInputArchive inArchive(is);

				inArchive(*t.get());

				t->fullPathName = fullPathName;
			}
		}

		~FileOperator();
	};
}