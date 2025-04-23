def run(task, **kwargs):
    if task == "eda":
        from eda.ohlcv_visualizer import visualize
        visualize(**kwargs)
    else:
        raise ValueError(f"Unknown task: {task}")