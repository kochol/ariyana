#pragma once

namespace ari
{
	template <class Treturn>
	class DelegateNoParam
	{
		class BaseFuncNoParam
		{
		public:
			virtual ~BaseFuncNoParam() = default;

			virtual Treturn Call() = 0;
		};

		template <class Tclass>
		class MemFuncNoParam : public BaseFuncNoParam
		{
		public:

			MemFuncNoParam(Tclass* _obj, Treturn(Tclass::*_fun)()) : m_pObj(_obj), m_pFun(_fun) {}

			Treturn Call() override
			{
				return (*m_pObj.*m_pFun)();
			}

		protected:

			Tclass	*	m_pObj;
			Treturn(Tclass::*m_pFun)();
		};

	public:

		DelegateNoParam() : m_pFun(nullptr), m_pMemFun(nullptr) { }

		~DelegateNoParam()
		{
			delete m_pMemFun;
		}

		void Bind(Treturn(*_fun)()) { m_pFun = _fun; }

		template <class Tclass>
		void Bind(Tclass* _obj, Treturn(Tclass::*_fun)())
		{
			m_pMemFun = new MemFuncNoParam<Tclass>(_obj, _fun);
		}

		bool IsBound() { return m_pFun || m_pMemFun; }

		Treturn Execute()
		{
			if (m_pFun)
				return m_pFun();
			if (m_pMemFun)
				return m_pMemFun->Call();
		}

	protected:
		Treturn(*m_pFun)();
		BaseFuncNoParam	*	m_pMemFun;
	};



	template <class Treturn, class Targ1>
	class DelegateOneParam
	{
		class BaseFuncOneParam
		{
		public:
			virtual ~BaseFuncOneParam() = default;

			virtual Treturn Call(Targ1 arg1) = 0;
		};

		template <class Tclass>
		class MemFuncOneParam : public BaseFuncOneParam
		{
		public:

			MemFuncOneParam(Tclass* _obj, Treturn(Tclass::*_fun)(Targ1)) : m_pObj(_obj), m_pFun(_fun) {}

			Treturn Call(Targ1 arg1) override
			{
				return (*m_pObj.*m_pFun)(arg1);
			}

		protected:

			Tclass	*	m_pObj;
			Treturn(Tclass::*m_pFun)(Targ1);
		};

	public:

		DelegateOneParam() : m_pFun(nullptr), m_pMemFun(nullptr) { }

		~DelegateOneParam()
		{
			delete m_pMemFun;
		}

		void Bind(Treturn(*_fun)(Targ1)) { m_pFun = _fun; }

		template <class Tclass>
		void Bind(Tclass* _obj, Treturn(Tclass::*_fun)(Targ1))
		{
			m_pMemFun = new MemFuncOneParam<Tclass>(_obj, _fun);
		}

		bool IsBound() { return m_pFun || m_pMemFun; }

		Treturn Execute(Targ1 arg1)
		{
			if (m_pFun)
				return m_pFun(arg1);
			if (m_pMemFun)
				return m_pMemFun->Call(arg1);
		}

	protected:
		Treturn(*m_pFun)(Targ1);
		BaseFuncOneParam	*	m_pMemFun;
	};



	template <class Treturn, class Targ1, class Targ2>
	class DelegateTwoParam
	{
		class BaseFuncTwoParam
		{
		public:
			virtual ~BaseFuncTwoParam() = default;

			virtual Treturn Call(Targ1 arg1, Targ2 arg2) = 0;
		};

		template <class Tclass>
		class MemFuncTwoParam : public BaseFuncTwoParam
		{
		public:

			MemFuncTwoParam(Tclass* _obj, Treturn(Tclass::*_fun)(Targ1, Targ2)) : m_pObj(_obj), m_pFun(_fun) {}

			Treturn Call(Targ1 arg1, Targ2 arg2) override
			{
				return (*m_pObj.*m_pFun)(arg1, arg2);
			}

		protected:

			Tclass	*	m_pObj;
			Treturn(Tclass::*m_pFun)(Targ1, Targ2);
		};

	public:

		DelegateTwoParam() : m_pFun(nullptr), m_pMemFun(nullptr) { }

		~DelegateTwoParam()
		{
			delete m_pMemFun;
		}

		void Bind(Treturn(*_fun)(Targ1, Targ2)) { m_pFun = _fun; }

		template <class Tclass>
		void Bind(Tclass* _obj, Treturn(Tclass::*_fun)(Targ1, Targ2))
		{
			m_pMemFun = new MemFuncTwoParam<Tclass>(_obj, _fun);
		}

		bool IsBound() { return m_pFun || m_pMemFun; }

		Treturn Execute(Targ1 arg1, Targ2 arg2)
		{
			if (m_pFun)
				return m_pFun(arg1, arg2);
			if (m_pMemFun)
				return m_pMemFun->Call(arg1, arg2);
		}

	protected:
		Treturn(*m_pFun)(Targ1, Targ2);
		BaseFuncTwoParam	*	m_pMemFun;
	};



	template <class Treturn, class Targ1, class Targ2, class Targ3>
	class DelegateThreeParam
	{
		class BaseFuncThreeParam
		{
		public:
			virtual ~BaseFuncThreeParam() = default;

			virtual Treturn Call(Targ1 arg1, Targ2 arg2, Targ3 arg3) = 0;
		};

		template <class Tclass>
		class MemFuncThreeParam : public BaseFuncThreeParam
		{
		public:

			MemFuncThreeParam(Tclass* _obj, Treturn(Tclass::*_fun)(Targ1, Targ2, Targ3)) : m_pObj(_obj), m_pFun(_fun) {}

			Treturn Call(Targ1 arg1, Targ2 arg2, Targ3 arg3) override
			{
				return (*m_pObj.*m_pFun)(arg1, arg2, arg3);
			}

		protected:

			Tclass	*	m_pObj;
			Treturn(Tclass::*m_pFun)(Targ1, Targ2, Targ3);
		};

	public:

		DelegateThreeParam() : m_pFun(nullptr), m_pMemFun(nullptr) { }

		~DelegateThreeParam()
		{
			delete m_pMemFun;
		}

		void Bind(Treturn(*_fun)(Targ1, Targ2, Targ3)) { m_pFun = _fun; }

		template <class Tclass>
		void Bind(Tclass* _obj, Treturn(Tclass::*_fun)(Targ1, Targ2, Targ3))
		{
			m_pMemFun = new MemFuncThreeParam<Tclass>(_obj, _fun);
		}

		bool IsBound() { return m_pFun || m_pMemFun; }

		Treturn Execute(Targ1 arg1, Targ2 arg2, Targ3 arg3)
		{
			if (m_pFun)
				return m_pFun(arg1, arg2, arg3);
			if (m_pMemFun)
				return m_pMemFun->Call(arg1, arg2, arg3);
		}

	protected:
		Treturn(*m_pFun)(Targ1, Targ2, Targ3);
		BaseFuncThreeParam	*	m_pMemFun;
	};



	template <class Treturn, class Targ1, class Targ2, class Targ3, class Targ4>
	class DelegateFourParam
	{
		class BaseFuncFourParam
		{
		public:
			virtual ~BaseFuncFourParam() = default;

			virtual Treturn Call(Targ1 arg1, Targ2 arg2, Targ3 arg3, Targ4 arg4) = 0;
		};

		template <class Tclass>
		class MemFuncFourParam : public BaseFuncFourParam
		{
		public:

			MemFuncFourParam(Tclass* _obj, Treturn(Tclass::*_fun)(Targ1, Targ2, Targ3, Targ4)) : m_pObj(_obj), m_pFun(_fun) {}

			Treturn Call(Targ1 arg1, Targ2 arg2, Targ3 arg3, Targ4 arg4) override
			{
				return (*m_pObj.*m_pFun)(arg1, arg2, arg3, arg4);
			}

		protected:

			Tclass	*	m_pObj;
			Treturn(Tclass::*m_pFun)(Targ1, Targ2, Targ3, Targ4);
		};

	public:

		DelegateFourParam() : m_pFun(nullptr), m_pMemFun(nullptr) { }

		~DelegateFourParam()
		{
			delete m_pMemFun;
		}

		void Bind(Treturn(*_fun)(Targ1, Targ2, Targ3, Targ4)) { m_pFun = _fun; }

		template <class Tclass>
		void Bind(Tclass* _obj, Treturn(Tclass::*_fun)(Targ1, Targ2, Targ3, Targ4))
		{
			m_pMemFun = new MemFuncFourParam<Tclass>(_obj, _fun);
		}

		bool IsBound() { return m_pFun || m_pMemFun; }

		Treturn Execute(Targ1 arg1, Targ2 arg2, Targ3 arg3, Targ4 arg4)
		{
			if (m_pFun)
				return m_pFun(arg1, arg2, arg3, arg4);
			if (m_pMemFun)
				return m_pMemFun->Call(arg1, arg2, arg3, arg4);
		}

	protected:
		Treturn(*m_pFun)(Targ1, Targ2, Targ3, Targ4);
		BaseFuncFourParam	*	m_pMemFun;
	};



	template <class Treturn, class Targ1, class Targ2, class Targ3, class Targ4, class Targ5>
	class DelegateFiveParam
	{
		class BaseFuncFiveParam
		{
		public:
			virtual ~BaseFuncFiveParam() = default;

			virtual Treturn Call(Targ1 arg1, Targ2 arg2, Targ3 arg3, Targ4 arg4, Targ5 arg5) = 0;
		};

		template <class Tclass>
		class MemFuncFiveParam : public BaseFuncFiveParam
		{
		public:

			MemFuncFiveParam(Tclass* _obj, Treturn(Tclass::*_fun)(Targ1, Targ2, Targ3, Targ4, Targ5)) : m_pObj(_obj), m_pFun(_fun) {}

			Treturn Call(Targ1 arg1, Targ2 arg2, Targ3 arg3, Targ4 arg4, Targ5 arg5) override
			{
				return (*m_pObj.*m_pFun)(arg1, arg2, arg3, arg4, arg5);
			}

		protected:

			Tclass	*	m_pObj;
			Treturn(Tclass::*m_pFun)(Targ1, Targ2, Targ3, Targ4, Targ5);
		};

	public:

		DelegateFiveParam() : m_pFun(nullptr), m_pMemFun(nullptr) { }

		~DelegateFiveParam()
		{
			delete m_pMemFun;
		}

		void Bind(Treturn(*_fun)(Targ1, Targ2, Targ3, Targ4, Targ5)) { m_pFun = _fun; }

		template <class Tclass>
		void Bind(Tclass* _obj, Treturn(Tclass::*_fun)(Targ1, Targ2, Targ3, Targ4, Targ5))
		{
			m_pMemFun = new MemFuncFiveParam<Tclass>(_obj, _fun);
		}

		bool IsBound() { return m_pFun || m_pMemFun; }

		Treturn Execute(Targ1 arg1, Targ2 arg2, Targ3 arg3, Targ4 arg4, Targ5 arg5)
		{
			if (m_pFun)
				return m_pFun(arg1, arg2, arg3, arg4, arg5);
			if (m_pMemFun)
				return m_pMemFun->Call(arg1, arg2, arg3, arg4, arg5);
		}

	protected:
		Treturn(*m_pFun)(Targ1, Targ2, Targ3, Targ4, Targ5);
		BaseFuncFiveParam	*	m_pMemFun;
	};



	template <class Treturn, class Targ1, class Targ2, class Targ3, class Targ4, class Targ5, class Targ6>
	class DelegateSixParam
	{
		class BaseFuncSixParam
		{
		public:
			virtual ~BaseFuncSixParam() = default;

			virtual Treturn Call(Targ1 arg1, Targ2 arg2, Targ3 arg3, Targ4 arg4, Targ5 arg5, Targ6 arg6) = 0;
		};

		template <class Tclass>
		class MemFuncSixParam : public BaseFuncSixParam
		{
		public:

			MemFuncSixParam(Tclass* _obj, Treturn(Tclass::*_fun)(Targ1, Targ2, Targ3, Targ4, Targ5, Targ6)) : m_pObj(_obj), m_pFun(_fun) {}

			Treturn Call(Targ1 arg1, Targ2 arg2, Targ3 arg3, Targ4 arg4, Targ5 arg5, Targ6 arg6) override
			{
				return (*m_pObj.*m_pFun)(arg1, arg2, arg3, arg4, arg5, arg6);
			}

		protected:

			Tclass	*	m_pObj;
			Treturn(Tclass::*m_pFun)(Targ1, Targ2, Targ3, Targ4, Targ5, Targ6);
		};

	public:

		DelegateSixParam() : m_pFun(nullptr), m_pMemFun(nullptr) { }

		~DelegateSixParam()
		{
			delete m_pMemFun;
		}

		void Bind(Treturn(*_fun)(Targ1, Targ2, Targ3, Targ4, Targ5, Targ6)) { m_pFun = _fun; }

		template <class Tclass>
		void Bind(Tclass* _obj, Treturn(Tclass::*_fun)(Targ1, Targ2, Targ3, Targ4, Targ5, Targ6))
		{
			m_pMemFun = new MemFuncSixParam<Tclass>(_obj, _fun);
		}

		bool IsBound() { return m_pFun || m_pMemFun; }

		Treturn Execute(Targ1 arg1, Targ2 arg2, Targ3 arg3, Targ4 arg4, Targ5 arg5, Targ6 arg6)
		{
			if (m_pFun)
				return m_pFun(arg1, arg2, arg3, arg4, arg5, arg6);
			if (m_pMemFun)
				return m_pMemFun->Call(arg1, arg2, arg3, arg4, arg5, arg6);
		}

	protected:
		Treturn(*m_pFun)(Targ1, Targ2, Targ3, Targ4, Targ5, Targ6);
		BaseFuncSixParam	*	m_pMemFun;
	};



	template <class Treturn, class Targ1, class Targ2, class Targ3, class Targ4, class Targ5, class Targ6, class Targ7>
	class DelegateSevenParam
	{
		class BaseFuncSevenParam
		{
		public:
			virtual ~BaseFuncSevenParam() = default;

			virtual Treturn Call(Targ1 arg1, Targ2 arg2, Targ3 arg3, Targ4 arg4, Targ5 arg5, Targ6 arg6, Targ7 arg7) = 0;
		};

		template <class Tclass>
		class MemFuncSevenParam : public BaseFuncSevenParam
		{
		public:

			MemFuncSevenParam(Tclass* _obj, Treturn(Tclass::*_fun)(Targ1, Targ2, Targ3, Targ4, Targ5, Targ6, Targ7)) : m_pObj(_obj), m_pFun(_fun) {}

			Treturn Call(Targ1 arg1, Targ2 arg2, Targ3 arg3, Targ4 arg4, Targ5 arg5, Targ6 arg6, Targ7 arg7) override
			{
				return (*m_pObj.*m_pFun)(arg1, arg2, arg3, arg4, arg5, arg6, arg7);
			}

		protected:

			Tclass	*	m_pObj;
			Treturn(Tclass::*m_pFun)(Targ1, Targ2, Targ3, Targ4, Targ5, Targ6, Targ7);
		};

	public:

		DelegateSevenParam() : m_pFun(nullptr), m_pMemFun(nullptr) { }

		~DelegateSevenParam()
		{
			delete m_pMemFun;
		}

		void Bind(Treturn(*_fun)(Targ1, Targ2, Targ3, Targ4, Targ5, Targ6, Targ7)) { m_pFun = _fun; }

		template <class Tclass>
		void Bind(Tclass* _obj, Treturn(Tclass::*_fun)(Targ1, Targ2, Targ3, Targ4, Targ5, Targ6, Targ7))
		{
			m_pMemFun = new MemFuncSevenParam<Tclass>(_obj, _fun);
		}

		bool IsBound() { return m_pFun || m_pMemFun; }

		Treturn Execute(Targ1 arg1, Targ2 arg2, Targ3 arg3, Targ4 arg4, Targ5 arg5, Targ6 arg6, Targ7 arg7)
		{
			if (m_pFun)
				return m_pFun(arg1, arg2, arg3, arg4, arg5, arg6, arg7);
			if (m_pMemFun)
				return m_pMemFun->Call(arg1, arg2, arg3, arg4, arg5, arg6, arg7);
		}

	protected:
		Treturn(*m_pFun)(Targ1, Targ2, Targ3, Targ4, Targ5, Targ6, Targ7);
		BaseFuncSevenParam	*	m_pMemFun;
	};



	template <class Treturn, class Targ1, class Targ2, class Targ3, class Targ4, class Targ5, class Targ6, class Targ7, class Targ8>
	class DelegateEightParam
	{
		class BaseFuncEightParam
		{
		public:
			virtual ~BaseFuncEightParam() = default;

			virtual Treturn Call(Targ1 arg1, Targ2 arg2, Targ3 arg3, Targ4 arg4, Targ5 arg5, Targ6 arg6, Targ7 arg7, Targ8 arg8) = 0;
		};

		template <class Tclass>
		class MemFuncEightParam : public BaseFuncEightParam
		{
		public:

			MemFuncEightParam(Tclass* _obj, Treturn(Tclass::*_fun)(Targ1, Targ2, Targ3, Targ4, Targ5, Targ6, Targ7, Targ8)) : m_pObj(_obj), m_pFun(_fun) {}

			Treturn Call(Targ1 arg1, Targ2 arg2, Targ3 arg3, Targ4 arg4, Targ5 arg5, Targ6 arg6, Targ7 arg7, Targ8 arg8) override
			{
				return (*m_pObj.*m_pFun)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
			}

		protected:

			Tclass	*	m_pObj;
			Treturn(Tclass::*m_pFun)(Targ1, Targ2, Targ3, Targ4, Targ5, Targ6, Targ7, Targ8);
		};

	public:

		DelegateEightParam() : m_pFun(nullptr), m_pMemFun(nullptr) { }

		~DelegateEightParam()
		{
			delete m_pMemFun;
		}

		void Bind(Treturn(*_fun)(Targ1, Targ2, Targ3, Targ4, Targ5, Targ6, Targ7, Targ8)) { m_pFun = _fun; }

		template <class Tclass>
		void Bind(Tclass* _obj, Treturn(Tclass::*_fun)(Targ1, Targ2, Targ3, Targ4, Targ5, Targ6, Targ7, Targ8))
		{
			m_pMemFun = new MemFuncEightParam<Tclass>(_obj, _fun);
		}

		bool IsBound() { return m_pFun || m_pMemFun; }

		Treturn Execute(Targ1 arg1, Targ2 arg2, Targ3 arg3, Targ4 arg4, Targ5 arg5, Targ6 arg6, Targ7 arg7, Targ8 arg8)
		{
			if (m_pFun)
				return m_pFun(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
			if (m_pMemFun)
				return m_pMemFun->Call(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
		}

	protected:
		Treturn(*m_pFun)(Targ1, Targ2, Targ3, Targ4, Targ5, Targ6, Targ7, Targ8);
		BaseFuncEightParam	*	m_pMemFun;
	};



	template <class Treturn, class Targ1, class Targ2, class Targ3, class Targ4, class Targ5, class Targ6, class Targ7, class Targ8, class Targ9>
	class DelegateNineParam
	{
		class BaseFuncNineParam
		{
		public:
			virtual ~BaseFuncNineParam() = default;

			virtual Treturn Call(Targ1 arg1, Targ2 arg2, Targ3 arg3, Targ4 arg4, Targ5 arg5, Targ6 arg6, Targ7 arg7, Targ8 arg8, Targ9 arg9) = 0;
		};

		template <class Tclass>
		class MemFuncNineParam : public BaseFuncNineParam
		{
		public:

			MemFuncNineParam(Tclass* _obj, Treturn(Tclass::*_fun)(Targ1, Targ2, Targ3, Targ4, Targ5, Targ6, Targ7, Targ8, Targ9)) : m_pObj(_obj), m_pFun(_fun) {}

			Treturn Call(Targ1 arg1, Targ2 arg2, Targ3 arg3, Targ4 arg4, Targ5 arg5, Targ6 arg6, Targ7 arg7, Targ8 arg8, Targ9 arg9) override
			{
				return (*m_pObj.*m_pFun)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
			}

		protected:

			Tclass	*	m_pObj;
			Treturn(Tclass::*m_pFun)(Targ1, Targ2, Targ3, Targ4, Targ5, Targ6, Targ7, Targ8, Targ9);
		};

	public:

		DelegateNineParam() : m_pFun(nullptr), m_pMemFun(nullptr) { }

		~DelegateNineParam()
		{
			delete m_pMemFun;
		}

		void Bind(Treturn(*_fun)(Targ1, Targ2, Targ3, Targ4, Targ5, Targ6, Targ7, Targ8, Targ9)) { m_pFun = _fun; }

		template <class Tclass>
		void Bind(Tclass* _obj, Treturn(Tclass::*_fun)(Targ1, Targ2, Targ3, Targ4, Targ5, Targ6, Targ7, Targ8, Targ9))
		{
			m_pMemFun = new MemFuncNineParam<Tclass>(_obj, _fun);
		}

		bool IsBound() { return m_pFun || m_pMemFun; }

		Treturn Execute(Targ1 arg1, Targ2 arg2, Targ3 arg3, Targ4 arg4, Targ5 arg5, Targ6 arg6, Targ7 arg7, Targ8 arg8, Targ9 arg9)
		{
			if (m_pFun)
				return m_pFun(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
			if (m_pMemFun)
				return m_pMemFun->Call(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
		}

	protected:
		Treturn(*m_pFun)(Targ1, Targ2, Targ3, Targ4, Targ5, Targ6, Targ7, Targ8, Targ9);
		BaseFuncNineParam	*	m_pMemFun;
	};



	template <class Treturn, class Targ1, class Targ2, class Targ3, class Targ4, class Targ5, class Targ6, class Targ7, class Targ8, class Targ9, class Targ10>
	class DelegateTenParam
	{
		class BaseFuncTenParam
		{
		public:
			virtual ~BaseFuncTenParam() = default;

			virtual Treturn Call(Targ1 arg1, Targ2 arg2, Targ3 arg3, Targ4 arg4, Targ5 arg5, Targ6 arg6, Targ7 arg7, Targ8 arg8, Targ9 arg9, Targ10 arg10) = 0;
		};

		template <class Tclass>
		class MemFuncTenParam : public BaseFuncTenParam
		{
		public:

			MemFuncTenParam(Tclass* _obj, Treturn(Tclass::*_fun)(Targ1, Targ2, Targ3, Targ4, Targ5, Targ6, Targ7, Targ8, Targ9, Targ10)) : m_pObj(_obj), m_pFun(_fun) {}

			Treturn Call(Targ1 arg1, Targ2 arg2, Targ3 arg3, Targ4 arg4, Targ5 arg5, Targ6 arg6, Targ7 arg7, Targ8 arg8, Targ9 arg9, Targ10 arg10) override
			{
				return (*m_pObj.*m_pFun)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10);
			}

		protected:

			Tclass	*	m_pObj;
			Treturn(Tclass::*m_pFun)(Targ1, Targ2, Targ3, Targ4, Targ5, Targ6, Targ7, Targ8, Targ9, Targ10);
		};

	public:

		DelegateTenParam() : m_pFun(nullptr), m_pMemFun(nullptr) { }

		~DelegateTenParam()
		{
			delete m_pMemFun;
		}

		void Bind(Treturn(*_fun)(Targ1, Targ2, Targ3, Targ4, Targ5, Targ6, Targ7, Targ8, Targ9, Targ10)) { m_pFun = _fun; }

		template <class Tclass>
		void Bind(Tclass* _obj, Treturn(Tclass::*_fun)(Targ1, Targ2, Targ3, Targ4, Targ5, Targ6, Targ7, Targ8, Targ9, Targ10))
		{
			m_pMemFun = new MemFuncTenParam<Tclass>(_obj, _fun);
		}

		bool IsBound() { return m_pFun || m_pMemFun; }

		Treturn Execute(Targ1 arg1, Targ2 arg2, Targ3 arg3, Targ4 arg4, Targ5 arg5, Targ6 arg6, Targ7 arg7, Targ8 arg8, Targ9 arg9, Targ10 arg10)
		{
			if (m_pFun)
				return m_pFun(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10);
			if (m_pMemFun)
				return m_pMemFun->Call(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10);
		}

	protected:
		Treturn(*m_pFun)(Targ1, Targ2, Targ3, Targ4, Targ5, Targ6, Targ7, Targ8, Targ9, Targ10);
		BaseFuncTenParam	*	m_pMemFun;
	};



}