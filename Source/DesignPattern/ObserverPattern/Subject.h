#pragma once
#include <BaseInclude.h>
#include "Obeserver.h"

#include VECTOR_INCLUDE_PATH

namespace Core
{
	class Subject
	{
	private:
		ctd::vector<Obeserver *> m_observerList;

	public:
		Subject();
		void Attach(Obeserver * pObserver);
		void Detach(Obeserver * pObserver);
		void NotifyAhead();
		void NotifyBehind();
		virtual ~Subject();
	};
}