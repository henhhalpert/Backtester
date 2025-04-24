def run(task, **kwargs):
    print("== RUNNER.py == ")
    if task == "eda":
        from eda.ohlcv_visualizer import visualize
        visualize(**kwargs)
    else:
        raise ValueError(f"Unknown task: {task}")

    

def main():
    # Example usage
    run("visualize")


if __name__ == "__main__":
    main()
