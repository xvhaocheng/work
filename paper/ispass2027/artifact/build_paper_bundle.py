#!/usr/bin/env python3
"""Create an anonymous source archive and compile a fresh independent extraction.

Only explicit publication-source inputs are copied. Private provenance and logs
stay outside the archive. Existing output directories are never overwritten.
"""
from __future__ import annotations

import argparse
import gzip
import hashlib
import io
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import tarfile
import tempfile


ARCHIVE_ROOT = "paper-source"
BUILD_COMMAND = ["make"]
README = """# Anonymous working-paper source package

This package compiles the current anonymous IEEE conference working draft.
It is not a completed submission or a release of the research artifact.

## Build

Run `make` from this directory. The output is `main.pdf`.
Run `make clean` to remove local compilation intermediates.

System dependencies: GNU Make, latexmk, pdfLaTeX, BibTeX and a standard
TeX Live installation providing fontenc, amsmath, amssymb, graphicx, booktabs,
array, xcolor, hyperref and balance, with Times/Nimbus Roman and AMS fonts.
On Debian/Ubuntu these are supplied by make, latexmk, texlive-latex-base,
texlive-latex-recommended, texlive-latex-extra and texlive-fonts-recommended.
No Python environment, network access, source repository, raw experiment data
or development-machine directory is required to compile this source package.

Unmodified IEEEtran.cls and IEEEtran.bst are bundled in the package root.
The local copies take precedence over installed copies. Their original notices
are retained; see THIRD-PARTY.txt and LPPL.txt. All other TeX dependencies
are supplied by the system distribution.

## Working-draft limits

The abstract, incomplete experiment panels and author-declaration gaps are
intentional and retained. Do not fill missing data from a drawn placeholder.
DS0 is original wiring; DS50 is added-delay sensitivity, not pure PTW cost.
Historical FPGA results, repaired-baseline measurements, capacity RTL simulation
and invalidation simulation have distinct evidence scopes. The manuscript
promises source/data release upon acceptance only. No upload or publication
is performed by creating this local package.

Normal third-person prior-work citations remain. Author identity is withheld.
Internal claim registries, raw paths, source audits and verification reports
are not part of the anonymous source archive. Invisible claim IDs in the TeX
are no-op markers and require no external file.

artifact-manifest.json lists the package files and their SHA256 values;
it does not include its own hash. The separate local packaging report records
source identity, actual compilation environment and validation results.
"""
MAKEFILE = """.PHONY: all clean
all:
\tlatexmk -pdf -pdflatex="pdflatex -no-shell-escape %O %S" -interaction=nonstopmode -halt-on-error main.tex
clean:
\tlatexmk -c main.tex
"""
THIRD_PARTY = """IEEEtran.cls V1.8b (2015/08/26) and IEEEtran.bst V1.14 (2015/08/26)
are copied unmodified from the system TeX distribution. Their copyright,
contributor, support and license notices remain in the files. Both identify
the LaTeX Project Public License version 1.3. LPPL.txt is copied from the
same TeX distribution. This source package includes the two files needed
to compile this manuscript, rather than the full IEEEtran documentation
and example collection. The complete IEEEtran distribution is available at:
https://ctan.org/pkg/ieeetran
https://www.michaelshell.org/tex/ieeetran/

The remaining files are the anonymous working-paper sources and generated
figure assets. Creating this local compilation package does not publish them
or grant a new redistribution license for the manuscript or research data.
"""


def sha(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def command(argv: list[str], cwd: Path | None = None, env=None) -> str:
    result = subprocess.run(argv, cwd=cwd, env=env, text=True,
                            stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    if result.returncode:
        raise RuntimeError(f"command failed ({result.returncode}): {argv[0]}\n{result.stdout[-5000:]}")
    return result.stdout


def version(argv: list[str]) -> str:
    return next(line.strip() for line in command(argv).splitlines() if line.strip())


def archive_bytes(files: dict[str, bytes]) -> bytes:
    target = io.BytesIO()
    with gzip.GzipFile(filename="", fileobj=target, mode="wb", mtime=0) as zipped:
        with tarfile.open(fileobj=zipped, mode="w", format=tarfile.PAX_FORMAT) as tar:
            for relative, data in sorted(files.items()):
                item = tarfile.TarInfo(f"{ARCHIVE_ROOT}/{relative}")
                item.size, item.mode, item.mtime = len(data), 0o644, 0
                item.uid = item.gid = 0
                item.uname = item.gname = ""
                tar.addfile(item, io.BytesIO(data))
    return target.getvalue()


def extract_archive(archive: Path, destination: Path) -> Path:
    with tarfile.open(archive, "r:gz") as tar:
        for member in tar.getmembers():
            relative = Path(member.name)
            if (not member.isfile() or relative.is_absolute() or ".." in relative.parts
                    or relative.parts[0] != ARCHIVE_ROOT):
                raise ValueError(f"unsafe/non-file archive member: {member.name}")
            target = destination / relative
            target.parent.mkdir(parents=True, exist_ok=True)
            stream = tar.extractfile(member)
            if stream is None:
                raise ValueError(f"missing archive member bytes: {member.name}")
            with target.open("xb") as out:
                out.write(stream.read())
    return destination / ARCHIVE_ROOT


def info_fields(text: str) -> dict[str, str]:
    return dict(line.split(":", 1) for line in text.splitlines() if ":" in line)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--output", type=Path, required=True,
                        help="New output directory; existing paths are refused")
    args = parser.parse_args()
    output = args.output.absolute()
    if output.exists() or output.is_symlink():
        parser.error("output already exists; choose a new attempt path")
    artifact = Path(__file__).resolve().parent
    paper, workspace = artifact.parent, artifact.parent.parents[1]
    if not output.resolve().is_relative_to(artifact / "packages"):
        parser.error("output must be a new directory under artifact/packages/")
    for executable in ("git", "kpsewhich", "make", "latexmk", "pdflatex",
                       "bibtex", "pdfinfo", "pdftotext", "pdftoppm"):
        if shutil.which(executable) is None:
            parser.error(f"missing system dependency: {executable}")
    output.mkdir(parents=True, exist_ok=False)
    report = {"schema": "anonymous-paper-bundle-validation-v1", "status": "building",
              "scope": "source packaging, independent extraction, TeX compilation and rendering only",
              "builder_sha256": sha(Path(__file__).read_bytes()),
              "source_commit_at_start": command(["git", "rev-parse", "HEAD"], workspace).strip(),
              "source_inputs": [], "package_contains_private_provenance": False}
    report_file = output / "validation.json"
    private_inputs: dict[Path, bytes] = {}
    files: dict[str, bytes] = {}

    def add(source: Path, relative: str) -> None:
        if relative in files:
            raise ValueError(f"duplicate package file: {relative}")
        data = source.read_bytes()
        files[relative], private_inputs[source] = data, data
        report["source_inputs"].append({"source": str(source), "package_path": relative,
                                        "sha256": sha(data), "bytes": len(data)})

    try:
        tex = paper / "tex"
        for name in ("main.tex", "references.bib", "numbers.tex"):
            add(tex / name, name)
        original_main = files["main.tex"]
        main_text = original_main.decode()
        if main_text.count("../analysis/figures/") != 2:
            raise ValueError("unexpected figure-path interface; inspect source before packaging")
        files["main.tex"] = main_text.replace("../analysis/figures/", "figures/").encode()
        report["main_transformation"] = "Replace exactly two ../analysis/figures/ prefixes with figures/; no other source edit"
        section_names = re.findall(r"\\input\{(sections/[A-Za-z0-9_-]+)\}", main_text)
        if len(section_names) != len(set(section_names)) or len(section_names) != 7:
            raise ValueError("unexpected section interface")
        for name in section_names:
            add(tex / (name + ".tex"), name + ".tex")
        figures = re.findall(r"\\paperfigure\{([^}]+)\}", "\n".join(
            files[name + ".tex"].decode() for name in section_names))
        if len(figures) != 10 or len(set(figures)) != 10:
            raise ValueError("expected ten distinct figure references")
        for name in figures:
            if not re.fullmatch(r"fig\d\d-[a-z-]+", name):
                raise ValueError("unexpected figure name")
            add(paper / "analysis/figures" / (name + ".pdf"), f"figures/{name}.pdf")
        dependencies = {}
        for name in ("IEEEtran.cls", "IEEEtran.bst"):
            source = Path(command(["kpsewhich", name]).strip())
            if not source.is_file():
                raise ValueError(f"cannot locate {name}")
            dependencies[name] = source
            add(source, name)
        lppl = dependencies["IEEEtran.cls"].parents[3] / "doc/latex/base/lppl.txt.gz"
        compressed = lppl.read_bytes()
        private_inputs[lppl] = compressed
        files["LPPL.txt"] = gzip.decompress(compressed)
        report["source_inputs"].append({"source": str(lppl), "package_path": "LPPL.txt",
                                        "source_sha256": sha(compressed),
                                        "uncompressed_sha256": sha(files["LPPL.txt"])})
        files.update({"README.md": README.encode(), "Makefile": MAKEFILE.encode(),
                      "THIRD-PARTY.txt": THIRD_PARTY.encode()})
        markers = (b"/home/", b"/Users/", b"xvhaocheng", b"work-root-", b"file://")
        for name, data in files.items():
            if any(marker in data for marker in markers):
                raise ValueError(f"private/development marker in package input: {name}")
        manifest = {"schema": "anonymous-paper-source-manifest-v1",
                    "scope": "compilation source package; working-draft gaps retained",
                    "files": [{"path": name, "sha256": sha(data), "bytes": len(data)}
                              for name, data in sorted(files.items())],
                    "transformations": {"main.tex": "package-local figures/ path only"},
                    "system_dependencies": ["make", "latexmk", "pdfLaTeX", "BibTeX",
                        "TeX Live packages/fonts listed in README.md"],
                    "third_party_files": ["IEEEtran.cls", "IEEEtran.bst", "LPPL.txt"],
                    "manifest_self_hash": "not included"}
        files["artifact-manifest.json"] = (json.dumps(manifest, indent=2) + "\n").encode()
        original_pdf = tex / "main.pdf"
        original_pdf_bytes = original_pdf.read_bytes()
        report["original_main_pdf_identity"] = {"path": str(original_pdf),
            "sha256": sha(original_pdf_bytes), "bytes": len(original_pdf_bytes),
            "role": "unchanged source-manuscript artifact; not replaced by derived PDF"}
        private_inputs[original_pdf] = original_pdf_bytes
        archive = output / "paper-source.tar.gz"
        packed = archive_bytes(files)
        with archive.open("xb") as handle:
            handle.write(packed)
        report["archive"] = {"file": archive.name, "sha256": sha(packed), "bytes": len(packed),
                             "files": len(files), "fixed_tar_owner_and_mtime": True}
        independent = Path(tempfile.mkdtemp(prefix="ispass-paper-unpack-"))
        extracted = extract_archive(archive, independent)
        for name, data in files.items():
            if (extracted / name).read_bytes() != data:
                raise ValueError(f"extraction content mismatch: {name}")
        env = os.environ.copy()
        for name in ("TEXINPUTS", "BIBINPUTS", "BSTINPUTS", "TEXMFOUTPUT"):
            env.pop(name, None)
        build_log = command(BUILD_COMMAND, extracted, env)
        (independent / "compile.log").write_text(build_log)
        log = (extracted / "main.log").read_text()
        if re.search(r"undefined|Rerun to get|Overfull", log):
            raise ValueError("unresolved citation/reference, rerun request or overfull box")
        pdf = extracted / "main.pdf"
        metadata = {k: v.strip() for k, v in info_fields(command(["pdfinfo", str(pdf)])).items()}
        if metadata.get("Author", "") or not metadata.get("Page size", "").startswith("612 x 792"):
            raise ValueError("unexpected anonymous metadata or paper size")
        text = command(["pdftotext", str(pdf), "-"])
        compact = " ".join(text.split())
        if any(marker.decode() in text for marker in markers) or re.search(r"pending%|pending cycles", text):
            raise ValueError("private marker or numerical placeholder in compiled PDF")
        for preserved in ("Anonymous Authors", "Draft gap.", "Author contributions", "released upon acceptance"):
            if preserved not in compact:
                raise ValueError(f"expected anonymous/draft statement missing: {preserved}")
        allowed_system = [Path(p) for p in ("/usr/share", "/usr/lib", "/var/lib/texmf", "/etc/texmf", "/var/cache")]
        system_inputs, bundle_inputs = set(), set()
        for line in (extracted / "main.fls").read_text().splitlines():
            if not line.startswith("INPUT "):
                continue
            path = Path(line[6:])
            path = (extracted / path).resolve() if not path.is_absolute() else path.resolve()
            if path.is_relative_to(extracted):
                bundle_inputs.add(str(path.relative_to(extracted)))
            elif any(path.is_relative_to(prefix) for prefix in allowed_system):
                system_inputs.add(str(path))
            else:
                raise ValueError(f"unlisted development/non-system TeX dependency: {path}")
        if "IEEEtran.cls" not in bundle_inputs or "The style file: IEEEtran.bst" not in (extracted / "main.blg").read_text():
            raise ValueError("bundled IEEEtran class/style were not used")
        review = independent / "review"
        review.mkdir()
        command(["pdftoppm", "-r", "100", "-png", str(pdf), str(review / "page")])
        rendered = sorted(review.glob("page-*.png"))
        if len(rendered) != int(metadata["Pages"]):
            raise ValueError("rendered page count mismatch")
        derived = pdf.read_bytes()
        with (output / "compiled.pdf").open("xb") as handle:
            handle.write(derived)
        unchanged = all(path.read_bytes() == data for path, data in private_inputs.items())
        if not unchanged:
            raise ValueError("source input changed during packaging; retain attempt and retry explicitly")
        report.update(status="passed", extraction_directory=str(extracted),
                      compile_command=BUILD_COMMAND, compile_directory=str(extracted),
                      build_log=str(independent / "compile.log"),
                      environment={name: version(argv) for name, argv in {
                          "latexmk": ["latexmk", "-v"], "pdflatex": ["pdflatex", "--version"],
                          "bibtex": ["bibtex", "--version"], "make": ["make", "--version"]}.items()},
                      metadata=metadata, source_inputs_unchanged=True,
                      derived_pdf={"file": "compiled.pdf", "sha256": sha(derived), "bytes": len(derived)},
                      tex_input_closure={"bundle_files": sorted(bundle_inputs), "system_files": sorted(system_inputs),
                                         "unlisted_inputs": []},
                      log_checks={"undefined": False, "overfull": False,
                          "underfull_hbox": len(re.findall(r"Underfull \\hbox", log)),
                          "underfull_vbox": len(re.findall(r"Underfull \\vbox", log))},
                      render={"dpi": 100, "pages": len(rendered), "directory": str(review),
                              "visual_review": "rendered; model inspection pending"},
                      retained_gaps=["working abstract", "DS25/DS100", "device count", "workload/size",
                          "verified page-size matrix", "latency distributions", "repaired mitigation rerun",
                          "author contributions, funding, conflict-of-interest facts"])
    except Exception as error:
        report.update(status="failed", error=f"{type(error).__name__}: {error}")
        report_file.write_text(json.dumps(report, indent=2) + "\n")
        raise
    report_file.write_text(json.dumps(report, indent=2) + "\n")
    print(json.dumps({"status": report["status"], "archive": report["archive"],
                      "derived_pdf": report["derived_pdf"], "pages": report["metadata"]["Pages"],
                      "validation": str(report_file), "review": report["render"]["directory"]}, indent=2))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
