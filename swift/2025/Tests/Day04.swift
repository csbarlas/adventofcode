import Testing

@testable import AdventOfCode

// Make a copy of this file for every day to ensure the provided smoke tests
// pass.
struct Day04Tests {
    // Smoke test data provided in the challenge question
    let testData = """
        ..@@.@@@@.
        @@@.@.@.@@
        @@@@@.@.@@
        @.@@@@..@.
        @@.@@@@.@@
        .@@@@@@@.@
        .@.@.@.@@@
        @.@@@.@@@@
        .@@@@@@@@.
        @.@.@@@.@.
        """

    @Test func testPart1() async throws {
        let challenge = Day04(data: testData)
        try await #expect(String(describing: challenge.part1()) == "13")
    }

    @Test func testPart2() async throws {
        let challenge = Day03(data: testData)
        try await #expect(String(describing: challenge.part2()) == "3121910778619")
    }
}
