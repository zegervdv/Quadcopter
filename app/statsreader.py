import pstats
p = pstats.Stats('openlp.prof')
p.sort_stats('tottime').print_stats(20)