# Change log
- control line 54 CSE
- control line 35 add stride offest 16 <- 1319841
                             offdet 32 <- 1319845


- MD r[k] = 0.0 replace loop by memeset; offset 16 <- tested
- MD delta_r[k] = 0.0 replace loop by memeset; offset 16 <- tested
- MD force function pow(r, 3.0) or (r*r*r)<- tested, no significant difference
- utili functions BOTH inline and not inline <- tested, no difference
- MD line 52 output dep fixed <- tested till here
- MD line 59 loop interchanged: l, i, j <- tested
- MD line 113 loop interchanged: l, i <- tested
              combine two loops <- tested, no difference
- MD line 120 loop interchanged: l, i <- tested
- ABANDONED MD line 95 if-else reduced <- tested: worse SEE 1317821
- MD line 95 if-else move out <- tested: faster SEE 1318047
- MD line 95 if-else use temp <- tested
- ABANDONED  control calloc replace by _mm_malloc <- cannot run
- MD line 86 loop interchanged: l, i, j <- tested: slower. reason: more if-else produced
- MD line 146 add_norm use restrict <- cannot compile
- MD function use const int <- tested, no difference see 1319966
- MD line 43 unroll add_norm <- tested, slower see 1319998

- Ready for profiling <- only shit

- New flag tried: -fp <- tested, included in -Ofast
                  -fast <- error: lm not found
                  add -ipo <- great improvement: 1320491

