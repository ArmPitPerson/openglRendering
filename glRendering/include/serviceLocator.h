///  by Carl Findahl (C) 2018
/// A Kukon Development Project

#ifndef SERVICELOCATOR_H
#define SERVICELOCATOR_H


template<typename T>
class ServiceLocator {
public:
	ServiceLocator() = default;

	// Get the service
	static T* get() { return mService; }

	// Provide a service
	static void provide(T* service) { mService = service; }

private:
	// The provided service of type T
	inline static T* mService = nullptr;
	
};


#endif // SERVICELOCATOR_H
