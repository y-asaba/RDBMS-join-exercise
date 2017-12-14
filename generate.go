package main

import (
	"fmt"
	"math/rand"
	"os"
	"time"

	"github.com/urfave/cli"
)

func generateData(count int, max int) {
	fmt.Println(count)
	for i := 0; i < count; i++ {
		fmt.Println(rand.Intn(max))
	}
}

func main() {
	app := cli.NewApp()
	app.Name = "generate"
	app.Usage = ""
	app.Flags = []cli.Flag{
		cli.IntFlag{
			Name:  "count, c",
			Value: 10,
		},
		cli.IntFlag{
			Name:  "max, m",
			Value: 10000,
		},
	}
	app.Action = func(c *cli.Context) error {
		rand.Seed(time.Now().UnixNano())
		count := c.Int("count")
		max := c.Int("max")

		// generate outer table
		generateData(count, max)

		// generate inner table
		generateData(count, max)

		return nil
	}

	app.Run(os.Args)
}
