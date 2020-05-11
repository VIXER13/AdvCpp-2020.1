#include "Coroutine.hpp"
#include <ucontext.h>
#include <queue>
#include <memory>

namespace coroutine
{

struct Routine;

namespace {

thread_local struct Ordinator
{
	static constexpr size_t STACK_SIZE = 1 << 16;
    ucontext_t ctx{};
	std::vector<Routine> routines;
	std::queue<routine_t> finished;
    routine_t current = 0;
} ordinator;

void entry();

}

struct Routine {
    ucontext_t ctx;
	RoutineFunction func;
	std::unique_ptr<uint8_t[]> stack;
    std::exception_ptr exception;
	bool finished = false;

	void reset(const RoutineFunction& f) {
		func = f;
		finished = false;
		exception = {};
		makecontext(&ctx, entry, 0);
	}

	explicit Routine(const RoutineFunction& f) :
        func(f),
		stack(std::make_unique<uint8_t[]>(Ordinator::STACK_SIZE)) {
		ctx.uc_stack.ss_sp = stack.get();
		ctx.uc_stack.ss_size = Ordinator::STACK_SIZE;
		ctx.uc_link = &ordinator.ctx;
		getcontext(&ctx);
		makecontext(&ctx, entry, 0);
	}

	Routine(const Routine&) = delete;
	Routine& operator=(const Routine&) = delete;

	Routine(Routine&&) = default;
	Routine& operator=(Routine&&) = default;
};

routine_t create(const RoutineFunction& function) {
	if (ordinator.finished.empty()) {
		ordinator.routines.emplace_back(function);
		return ordinator.routines.size();
	} else {
		routine_t id = ordinator.finished.front();
		ordinator.finished.pop();
		Routine& routine = ordinator.routines[id - 1];
		routine.reset(function);
		return id;
	}
}

bool resume(routine_t id) {
	if (id == 0 || id > ordinator.routines.size())
		return false;

	const Routine& routine = ordinator.routines[id - 1];
	if (routine.finished)
		return false;

	ordinator.current = id;
	if (swapcontext(&ordinator.ctx, &routine.ctx) < 0) {
		ordinator.current = 0;
		return false;
	}

	if (routine.exception)
		std::rethrow_exception(routine.exception);

	return true;
}

void yield() {
	routine_t id = ordinator.current;
	Routine& routine = ordinator.routines[id - 1];
	ordinator.current = 0;
	swapcontext(&routine.ctx, &ordinator.ctx);
}

routine_t current() {
	return ordinator.current;
}

namespace {

void entry() {
	routine_t id = ordinator.current;
	Routine& routine = ordinator.routines[id - 1];

	if (routine.func) try {
		routine.func();
	} catch (...) {
		routine.exception = std::current_exception();
	}

	routine.finished = true;
	ordinator.current = 0;
	ordinator.finished.emplace(id);
}

}

}  // namespace coroutine
