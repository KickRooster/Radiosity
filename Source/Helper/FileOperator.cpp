#include "FileOperator.h"
#include <SDL\SDL_rwops.h>

namespace Core
{
	FileOperator::FileOperator()
	{
	}

	void * FileOperator::MapMemory(const ANSICHAR* fileFullPathName, Bool binaryFile)
	{
		SDL_RWops *rw = SDL_RWFromFile(fileFullPathName, binaryFile ? "rb" : "r");
		
		if (rw == Null)
			return Null;

		int64 res_size = SDL_RWsize(rw);
		ANSICHAR * res = new ANSICHAR[binaryFile ? res_size : res_size + 1];

		int64 nb_read_total = 0;
		int64 nb_read = 1;

		ANSICHAR* buf = res;

		while (nb_read_total < res_size && nb_read != 0)
		{
			nb_read = SDL_RWread(rw, buf, 1, (res_size - nb_read_total));
			nb_read_total += nb_read;
			buf += nb_read;
		}

		SDL_RWclose(rw);

		if (nb_read_total != res_size)
		{
			delete[] res;

			return Null;
		}

		if (!binaryFile)
			res[nb_read_total] = '\0';

		return res;
	}

	void FileOperator::UnmapMemory(void * pBytes)
	{
		if (pBytes != Null)
			delete[] pBytes;
	}

	FileOperator::~FileOperator()
	{

	}

}