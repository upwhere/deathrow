deathrow
========

Auto-maintaining fixed-size queue-like data-structure, useful for maintaining sequential histories.

The `deathrow` is created by specifying a size and passing in its first data, it can then be added to normally until the size runs out. When the size has run out and another element is added, the last element (FIFO) is automatically removed.

Look at the shitty spec:
- Insert: `O(N)`
- Remove: `N/A`
- Search: `O(N)`

