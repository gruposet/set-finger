$(function () {
    $('#container').highcharts({
        chart: {
            type: 'area'
        },
        title: user,
        xAxis: {
            categories: [
                    'Segunda',
                'Terça',
                'Quarta',
                'Quinta',
                'Sexta']
        },
        yAxis: {
            title: {
                text: 'Horas'
            }
        },
        credits: {
            enabled: false
        },
        series: dados
    });
});
