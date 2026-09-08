"""
编译并运行数组、交互流程与文件输出的回归测试。

运行：python tests/run_tests.py。需要 GCC，测试产物均位于临时目录。
"""
from pathlib import Path
from os import environ
from re import findall
from subprocess import run as sub_run
from tempfile import TemporaryDirectory, gettempdir

ROOT = Path(__file__).resolve().parents[1]
FLAGS = ["gcc", "-std=c17", "-Wall", "-Wextra", "-Wpedantic", "-Werror"]
FLAGS += [
    f"-I{ROOT / directory}"
    for directory
    in ("inc/core", "inc/io/in", "inc/io/out", "inc/utils")
]


def command(args: list[str]) -> None:
    """
    执行编译或测试命令。

    Args:
        args (list[str]): 命令及其参数组成的字符串列表。

    Raises:
        subprocess.CalledProcessError: 命令以非零状态退出。
        subprocess.TimeoutExpired: 命令运行超过 60 秒。
        OSError: 无法启动命令。
    """
    sub_run(args, check=True, timeout=60)


with TemporaryDirectory(prefix="phigros-tests-") as temporary:
    work = Path(temporary)
    assert work.resolve().parent == Path(gettempdir()).resolve()
    assert work.name.startswith("phigros-tests-")
    array_test = work / "test_array.exe"
    command(
        FLAGS
        + [
            "-Drealloc=test_realloc", str(ROOT / "tests/test_array.c"),
            str(ROOT / "src/utils/dynamic_array.c"), "-o", str(array_test)
        ]
    )
    command([str(array_test)])
    # 修改显示名称后，特殊选项与范围仍由 InputKind 决定。
    input_test = work / "test_input.exe"
    command(
        FLAGS + [
            str(ROOT / "tests/test_input.c"),
            str(ROOT / "src/io/in/data_setting.c"),
            str(ROOT / "src/utils/print.c"),
            str(ROOT / "src/utils/tools.c"),
            "-o", str(input_test),
        ]
    )
    sub_run(
        [str(input_test)], input="-1\n0\n0\n1\n", encoding="utf-8",
        capture_output=True, check=True, timeout=10,
    )
    for header in (ROOT / "inc").rglob("*.h"):
        # 每个头文件单独编译，避免依赖其他头文件的包含顺序。
        unit = work / "header.c"
        unit.write_text(f'#include "{header.as_posix()}"\n', encoding="utf-8")
        command(FLAGS + ["-fsyntax-only", str(unit)])
    
    executable = work / "calculator.exe"
    sources = [ROOT / "main.c", *sorted((ROOT / "src").rglob("*.c"))]
    command(
        FLAGS
        + [
            "-Drealloc=test_realloc", "-Dfree=test_free",
            *map(str, sources), str(ROOT / "tests/allocation_probe.c"),
            "-o", str(executable), "-lm"
        ]
    )

    def run(data: str, expected: int = 0, fail_at: int | None = None) -> str:
        """
        用预设输入运行程序并检查退出状态。

        Args:
            data (str): 写入标准输入的文本，发送完毕后关闭输入流。
            expected (int): 预期的整数退出码，默认为 0。
            fail_at (int | None): 指定第几次分配失败，从 1 开始；None 表示不注入失败。

        Returns:
            text (str): 程序标准输出的文本。

        Raises:
            AssertionError: 实际退出码与预期不符。
            subprocess.TimeoutExpired: 程序运行超过 10 秒。
            OSError: 无法启动程序。
        """
        env = environ.copy()
        env.pop("PHIGROS_FAIL_AT", None)
        if fail_at is not None:
            env["PHIGROS_FAIL_AT"] = str(fail_at)
        
        result = sub_run(
            [str(executable)], input=data, encoding="utf-8",
            capture_output=True, cwd=work, env=env, timeout=10
        )
        
        assert result.returncode == expected, (result.returncode, result.stderr)
        return result.stdout

    assert "Perfect：990" in run("\n1000\n900000\n1\n1\nx\nx\n")
    assert "未找到" in run("\n10\n123456\n")
    for goal, text in [(0, "放置即可"), (685000, "全部 good"), (1000000, "获得 AP")]:
        assert text in run(f"\n1000\n{goal}\n")
    
    assert "Perfect：990" in run("\n1000\n-1\n1000\n900000\n")
    assert "获得 AP" in run("\n1000\n10000000\n1000000\n")
    
    # 连续无解后再次计算，退出时由 allocation_probe 检查数组是否释放。
    run("\n10\n123456\n\n" * 5 + "\n1000\n900000\n")
    for data in (
        "", "\n",
        "\n1000\n", "\n1000\n900000\n",
        "\n1000\n900000\n1\n", "\n1000\n900000\n0\n"
    ):
        run(data)
        
    # 分别模拟初始化失败、扩容失败，再验证大量方案下的正常扩容。
    run("\n1000\n900000\n", expected=1, fail_at=1)
    run("\n5000\n500000\n", expected=1, fail_at=2)
    run("\n5000\n500000\n")
    run("\n1000\n900000\n0\n\nx\nx\n")
    exported = work / "solutions/default/1000notes,goal=900000.txt"
    text = exported.read_text(encoding="utf-8")
    solutions = findall(r"Perfect：(\d+)\s*, Good：(\d+)\s*, Max Combo：(\d+)", text)
    assert len(solutions) == 810
    
    # 校验导出的实际组合，不能只检查文件是否创建。
    for row in solutions:
        perfect, good, combo = map(int, row)
        assert 900000 * perfect + 585000 * good + 100000 * combo == 900000 * 1000
        assert 1 <= combo <= perfect + good <= 1000
        assert (perfect + good - 1) // combo <= 1000 - perfect - good
        
    exported.write_text("keep me", encoding="utf-8")
    run("\n1000\n900000\n0\n\nn\nx\nx\n")
    assert exported.read_text(encoding="utf-8") == "keep me"
    run("\n1000\n900000\n0\n\nr\nx\nx\n")
    assert exported.read_text(encoding="utf-8") == text
    run("\n1000\n900000\n0\none/two\nx\nx\n")
    assert (work / "solutions/one/two/1000notes,goal=900000.txt").exists()
    run("\n1000\n900000\n0\n0\nx\nx\n")
    (work / "solutions/blocked").write_text("not a directory", encoding="utf-8")
    run("\n1000\n900000\n0\nblocked\n", expected=1)

print("PASS: strict build, headers, allocation failures/cleanup, input, repeated rounds and export")
