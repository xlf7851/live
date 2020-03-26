#pragma once
#include <mutex>
#include <memory>

namespace xlf
{
	template<typename T>
	class TSingleton {
	public:

		//��ȡȫ�ֵ�������
		template<typename ...Args>
		static std::shared_ptr<T> Instance(Args&&... args) {
			if (!m_pSington) {
				std::lock_guard<std::mutex> gLock(m_Mutex);
				if (nullptr == m_pSington) {
					m_pSington = std::make_shared<T>(std::forward<Args>(args)...);
				}
			}
			return m_pSington;
		}

		//����������������һ�㲻��Ҫ����������������������
		static void DesInstance() {
			if (m_pSington) {
				m_pSington.reset();
				m_pSington = nullptr;
			}
		}

	private:
		explicit TSingleton();
		TSingleton(const TSingleton&) = delete;
		TSingleton& operator=(const TSingleton&) = delete;
		~TSingleton();

	private:
		static std::shared_ptr<T> m_pSington;
		static std::mutex m_Mutex;
	};

	template<typename T>
	std::shared_ptr<T> TSingleton<T>::m_pSington = nullptr;

	template<typename T>
	std::mutex TSingleton<T>::m_Mutex;
	

}

