Psychic Octo Robot
------------------

POR is an experiment with lexing out a stack based machine.

Native instructions - there are none. How's that for small?

Words that are not understood are pushed onto the stack as text.
That should make debugging terribly difficult.

Conditionals are what I'm interested in. Can I program
words to accept:

  if ... (else ...) endif

It should work that when 'if' is encountered, we test the
top of the stack. If the condition is true, we will execute
to the else and then skip to the endif. Otherwise, we will
execute from the else to the endif.

