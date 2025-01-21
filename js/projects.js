document.addEventListener('DOMContentLoaded', function() {
    const searchInput = document.getElementById('search-input');
    const searchButton = document.getElementById('search-button');
    const clearIcon = document.getElementById('clear-icon');
    const projectsContainer = document.getElementById('projects-container');
    const projects = projectsContainer.getElementsByClassName('card');

    function filterProjects() {
        const filter = searchInput.value.toLowerCase();
        for (let i = 0; i < projects.length; i++) {
            const title = projects[i].querySelector('.title h2').innerText.toLowerCase();
            const description = projects[i].querySelector('.description').innerText.toLowerCase();
            if (title.includes(filter) || description.includes(filter)) {
                projects[i].style.display = '';
            } else {
                projects[i].style.display = 'none';
            }
        }
        clearIcon.style.display = filter ? 'inline-block' : 'none';
    }

    searchInput.addEventListener('input', filterProjects);

    searchButton.addEventListener('click', filterProjects);

    clearIcon.addEventListener('click', function() {
        searchInput.value = '';
        filterProjects();
    });

    searchInput.addEventListener('keypress', function(event) {
        if (event.key === 'Enter') {
            filterProjects();
        }
    });
});