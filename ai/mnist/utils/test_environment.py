#!/usr/bin/python3

# https://stackoverflow.com/questions/62037787/sharing-conda-environments-over-git-what-are-the-best-practices

from subprocess import run
from pathlib import Path
import yaml
import difflib

def test_conda_environment():
    """Compares output of ``conda env export`` with the contents of ``conda_env.yml``. """

    # execute 'conda env export' and parse yaml output:
    cmd_result = run(['conda', 'env', 'export'], capture_output=True)
    d0 = yaml.safe_load(cmd_result.stdout.decode('utf-8'))

    # read saved conda environment from yaml file:
    fn = Path('.') / 'conda_env.yml'
    with open(fn, 'rt') as fp:
        d1 = yaml.safe_load(fp)

    # Compare the two dictionaries:
    if d0['channels'] != d1['channels']:
        print(f"Conda channels differ (current vs '{fn}'): " +
              f"{d0['channels']} vs {d0['channels']}")

    s0, s1 = sorted(d0['dependencies']), sorted(d1['dependencies'])
    if s0 != s1:
        df = difflib.Differ()
        ds = df.compare(s0, s1)
        ts = f"Differences of current environment (+) and file '{fn}' (-):"
        print("\n" + ts)
        print('=' * len(ts))
        print('\n'.join(l_ for l_ in ds if not l_.startswith(' ')))
        assert False  # fail test
    else:
        print("no issue found")

if __name__ ==  "__main__":
	test_conda_environment();

