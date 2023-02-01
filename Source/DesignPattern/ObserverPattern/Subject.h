#pragma once
#include <BaseInclude.h>
#include "Obeserver.h"

#include <vector>

namespace Core
{
	class Subject
	{
	private:
		std::vector<Obeserver *> m_observerList;

	public:
		Subject();
		void Attach(Obeserver * pObserver);
		void Detach(Obeserver * pObserver);
		void NotifyAhead();
		void NotifyBehind();
		virtual ~Subject();
	};
}