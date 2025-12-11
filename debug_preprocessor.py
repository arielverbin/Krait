import os
import shutil

import re

class GCDebuggerModule:
    pattern = re.compile(
        r"gc::make_tracked<(?P<type>[^>]+)>\((?P<args>[^)]*)"
    )

    def preprocess(self, filename, data):
        def replacer(match):
            t = match.group("type").strip()
            args = match.group("args").strip()
            
            # Avoid double-inserting
            if "__FILE__" in args or "__LINE__" in args:
                return match.group(0)

            new_args = f"__FILE__, __LINE__"
            if args:
                new_args += f", {args}"

            return f"gc::make_tracked<{t}>({self.pattern.sub(replacer, new_args)}"

        return self.pattern.sub(replacer, data)


class Preprocessor:
    def __init__(self, modules):
        self.modules = modules
        self.cache_root = "./preprocessor-cache"

    def _iter_files(self):
        for root, _, files in os.walk("."):
            # Ignore cache folder itself
            if self.cache_root in root:
                continue

            for f in files:
                if f.endswith((".cpp", ".hpp", ".h", ".cc")):
                    full = os.path.join(root, f)
                    yield full

    def preprocess(self):
        if os.path.exists(self.cache_root):
            print(f"Cache folder '{self.cache_root}' already exists. "
              f"Aborting preprocessing to avoid overwriting.")
            return

        for path in self._iter_files():
            # read original
            with open(path, "r", encoding="utf-8") as f:
                data = f.read()

            # save backup
            cache_path = os.path.join(self.cache_root, path.lstrip("./"))
            os.makedirs(os.path.dirname(cache_path), exist_ok=True)
            with open(cache_path, "w", encoding="utf-8") as f:
                f.write(data)

            # apply modules
            for module in self.modules:
                data = module.preprocess(path, data)

            # write final result
            with open(path, "w", encoding="utf-8") as f:
                f.write(data)

    def revert(self):
        if not os.path.exists(self.cache_root):
            print("No cache to revert.")
            return

        for root, _, files in os.walk(self.cache_root):
            for f in files:
                cache_path = os.path.join(root, f)
                rel = os.path.relpath(cache_path, self.cache_root)
                orig_path = os.path.join(".", rel)

                os.makedirs(os.path.dirname(orig_path), exist_ok=True)
                shutil.copyfile(cache_path, orig_path)

        shutil.rmtree(self.cache_root)
        print("Reverted successfully.")

if __name__ == "__main__":
    p = Preprocessor(modules=[
        GCDebuggerModule(),
    ])

    import sys
    if len(sys.argv) > 1 and sys.argv[1] == "revert":
        p.revert()
    else:
        p.preprocess()