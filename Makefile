
all: compile_demo

compile_demo: demo.cpp
	g++ demo.cpp -o demo.out

demo_test:
	make demo
	./demo.out

# old testing from class

TEST_CASES=tests/01.output tests/02.output tests/03.output tests/04.output tests/05.output tests/06.output

std: solution_std.cpp
	g++ -o solution.out $<

custom: solution_custom.cpp
	g++ -o solution.out $<

tests/%.output: tests/%.input solution.out
	@./solution.out < $< > $@ || (echo "Failed to run test case: '$<'" && rm $@)

test_old: $(TEST_CASES) check

check:
	@for i in tests/*.expected ; do \
		printf "Checking $$i... " ; \
	    diff -E $$i "tests/$$(basename "$$i" .expected).output" && echo "✔️"; \
	done || (echo "Some test cases did not pass." && exit 1)

clean:
	rm -f solution.out 2>/dev/null
	rm -f $(TEST_CASES) 2>/dev/null
