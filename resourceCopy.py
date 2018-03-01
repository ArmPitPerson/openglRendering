import distutils
from distutils import dir_util # Needed for some reason
import argparse


def main():
    # Set up argument parsing
    parser = argparse.ArgumentParser()
    parser.add_argument("source", type=str, help="Source file/dir")
    parser.add_argument("destination", type=str, help="Destination file/dir")
    args = parser.parse_args()

    # Copy the files
    print(f"COPYING:\n{args.source} => {args.destination}")
    distutils.dir_util.copy_tree(args.source, args.destination)


if __name__ == '__main__':
    main()
