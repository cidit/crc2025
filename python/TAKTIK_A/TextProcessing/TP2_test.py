from TP2 import solve

def test_document1():
    assert(solve('edcba') == ['a', 'e', 'ab', 'ac', 'ad', 'ae', 'ba', 'be', 'ca', 'ce', 'da', 'de', 'ea', 'eb', 'ec', 'ed', 'abc', 'abd', 'abe', 'acb', 'acd', 'ace', 'adb', 'adc', 'ade', 'aeb', 'aec', 'aed', 'bac', 'bad', 'bae', 'bca', 'bce', 'bda', 'bde', 'bea', 'bec', 'bed', 'cab', 'cad', 'cae', 'cba', 'cbe', 'cda', 'cde', 'cea', 'ceb', 'ced', 'dab', 'dac', 'dae', 'dba', 'dbe', 'dca', 'dce', 'dea', 'deb', 'dec', 'eab', 'eac', 'ead', 'eba', 'ebc', 'ebd', 'eca', 'ecb', 'ecd', 'eda', 'edb', 'edc', 'abce', 'abde', 'abec', 'abed', 'acbe', 'acde', 'aceb', 'aced', 'adbe', 'adce', 'adeb', 'adec', 'aebc', 'aebd', 'aecb', 'aecd', 'aedb', 'aedc', 'bacd', 'bace', 'badc', 'bade', 'baec', 'baed', 'bcad', 'bcae', 'bcea', 'bced', 'bdac', 'bdae', 'bdea', 'bdec', 'beac', 'bead', 'beca', 'becd', 'beda', 'bedc', 'cabd', 'cabe', 'cadb', 'cade', 'caeb', 'caed', 'cbad', 'cbae', 'cbea', 'cbed', 'cdab', 'cdae', 'cdea', 'cdeb', 'ceab', 'cead', 'ceba', 'cebd', 'ceda', 'cedb', 'dabc', 'dabe', 'dacb', 'dace', 'daeb', 'daec', 'dbac', 'dbae', 'dbea', 'dbec', 'dcab', 'dcae', 'dcea', 'dceb', 'deab', 'deac', 'deba', 'debc', 'deca', 'decb', 'eabc', 'eabd', 'eacb', 'eacd', 'eadb', 'eadc', 'ebac', 'ebad', 'ebca', 'ebda', 'ecab', 'ecad', 'ecba', 'ecda', 'edab', 'edac', 'edba', 'edca', 'abced', 'abdec', 'abecd', 'abedc', 'acbed', 'acdeb', 'acebd', 'acedb', 'adbec', 'adceb', 'adebc', 'adecb', 'bacde', 'baced', 'badce', 'badec', 'baecd', 'baedc', 'bcade', 'bcaed', 'bcead', 'bceda', 'bdace', 'bdaec', 'bdeac', 'bdeca', 'beacd', 'beadc', 'becad', 'becda', 'bedac', 'bedca', 'cabde', 'cabed', 'cadbe', 'cadeb', 'caebd', 'caedb', 'cbade', 'cbaed', 'cbead', 'cbeda', 'cdabe', 'cdaeb', 'cdeab', 'cdeba', 'ceabd', 'ceadb', 'cebad', 'cebda', 'cedab', 'cedba', 'dabce', 'dabec', 'dacbe', 'daceb', 'daebc', 'daecb', 'dbace', 'dbaec', 'dbeac', 'dbeca', 'dcabe', 'dcaeb', 'dceab', 'dceba', 'deabc', 'deacb', 'debac', 'debca', 'decab', 'decba', 'ebacd', 'ebadc', 'ebcad', 'ebdac', 'ecabd', 'ecadb', 'ecbad', 'ecdab', 'edabc', 'edacb', 'edbac', 'edcab'])

def test_document2():
    assert(solve('tests') == ['e', 'es', 'es', 'et', 'et', 'se', 'se', 'te', 'te', 'ess', 'est', 'est', 'ess', 'est', 'est', 'ets', 'ets', 'ett', 'ets', 'ets', 'ett', 'ses', 'set', 'set', 'sse', 'ste', 'ste', 'ses', 'set', 'set', 'sse', 'ste', 'ste', 'tes', 'tes', 'tet', 'tse', 'tse', 'tte', 'tes', 'tes', 'tet', 'tse', 'tse', 'tte', 'sest', 'sest', 'sets', 'sett', 'sets', 'sett', 'sset', 'sset', 'stes', 'stet', 'stes', 'stet', 'sest', 'sest', 'sets', 'sett', 'sets', 'sett', 'sset', 'sset', 'stes', 'stet', 'stes', 'stet', 'tess', 'test', 'tess', 'test', 'tets', 'tets', 'tses', 'tset', 'tses', 'tset', 'ttes', 'ttes', 'tess', 'test', 'tess', 'test', 'tets', 'tets', 'tses', 'tset', 'tses', 'tset', 'ttes', 'ttes', 'ssett', 'ssett', 'stest', 'stets', 'stest', 'stets', 'ssett', 'ssett', 'stest', 'stets', 'stest', 'stets', 'tsest', 'tsets', 'tsest', 'tsets', 'ttess', 'ttess', 'tsest', 'tsets', 'tsest', 'tsets', 'ttess', 'ttess'])

def test_document3():
    assert(solve('aeiou') == ['a', 'e', 'i', 'o', 'u', 'ae', 'ai', 'ao', 'au', 'ea', 'ei', 'eo', 'eu', 'ia', 'ie', 'io', 'iu', 'oa', 'oe', 'oi', 'ou', 'ua', 'ue', 'ui', 'uo']
)

    